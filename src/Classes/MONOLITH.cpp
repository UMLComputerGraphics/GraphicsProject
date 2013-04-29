/**
 @file MONOLITH.cpp
 @author UMLComputerGraphics <https://github.com/UMLComputerGraphics>
 @date 2013-03-29
 @brief This is a monolith of every component.
 @details Final project rough draft.
 Original engine based on Ed Angel's book code.
 **/

#include "MONOLITH.hpp"


MONOLITH::~MONOLITH(void)
{
    
}

/* Default and only constructor */
MONOLITH::MONOLITH(int argc, char** argv)
{
    _argc = argc;
    _argv = argv;
}

/**
 * Cleans up our scene graph.
 */
void MONOLITH::cleanup(void)
{
    Engine::instance()->rootScene()->delObject();
}

/**
 * Apply animations and whatever else your heart desires.
 */
void MONOLITH::monolith_idle(void)
{
    static Scene *rootScene = Engine::instance()->rootScene();
    
    // Animation variables.
    double timer = glutGet( GLUT_ELAPSED_TIME ) / 500.0;
    float percent = (sin( timer ) + 1.0) / 2.0;
    
    Object &candle = *((*rootScene)["bottle"]);
    candle.animation( simpleRotateAnim );

    //(*rootScene)["candle_top"]->morphPercentage(percent);

    /*
	  Object &candle_base = *((*rootScene)["candle_base"]);
	  Object &candle_top = *((*rootScene)["candle_top"]);
	  candle_base.animation( candleMeltAnim );
	  candle_top.animation(candleTopMeltDown);
	*/
    
    // Update the morph percentage.
    if((*rootScene)["bottle"]->morphEnabled())
    {
        (*rootScene)["bottle"]->morphPercentage(percent);
    }
}

#ifndef WITHOUT_QT
/**
 * Implementation of all slots and sighnals.
 * Qt-specific
 */
void MONOLITH::slotParticleAdd(int value)
{
//    int delta = value - ps->getNumParticles();
    ps->setNumParticles(value);
    printf("Particle system now has %d particles.\n", ps->getNumParticles());
}

void MONOLITH::slotFreezeParticles(bool isEnabled)
{
	ps->setPause(isEnabled);
}

void MONOLITH::slotMorphPercentage(int value)
{
    (*rootScene)["bottle"]->morphPercentage(value / 100.0);
}

void MONOLITH::slotEnableMorphing(bool isEnabled)
{
   (*rootScene)["bottle"]->morphEnabled(isEnabled);
}

#endif //WITHOUT_QT
/**
 * This will initialize and run MONOLITH
 */
void MONOLITH::run() {
  Engine *eng = Engine::instance();

  eng->init( &_argc, _argv, "WE ARE THE BORG. RESISTANCE IS FUTILE!" );
  eng->registerIdle( monolith_idle );
  eng->registerTraceFunc( (raytracerCallback)(boost::bind(&MONOLITH::raytraceStatusChanged, this, _1)));

  // Get handles to the Scene and the Screen.
  rootScene = Engine::instance()->rootScene();
  primScreen = Engine::instance()->mainScreen();
  
  // No Morphing, plus Light Shading.
  shader[1] = Angel::InitShader( "shaders/vEngineNoMorph.glsl",
				 "shaders/fEngine.glsl" );
  // Particle Shader.
  shader[2] = Angel::InitShader( "shaders/vParticle2.glsl",
                                 "shaders/fParticle2.glsl" );

  // Raytracing shader
  if (eng->glslVersion() >= 1.50)
    shader[3] = Angel::InitShader( "shaders/vRaytracer.glsl", 
				   "shaders/fRaytracer.glsl" );
  else
    shader[3] = 0;

  GLint morphingShader = Engine::instance()->rootScene()->shader(); 
  GLint noMorphShader = shader[1];
  GLint particleShader = shader[2];
  GLint raytraceShader = shader[3];

  tick.setTimeUniform( glGetUniformLocation( morphingShader, "ftime" ) );
  tick.setTimeUniform( glGetUniformLocation( noMorphShader, "ftime" ) );
  if (raytraceShader)
    tick.setTimeUniform( glGetUniformLocation( raytraceShader, "ftime" ) );

  // --- Wine Bottle --- //
  
  // Create the Bottle Object handle...
  bottle = rootScene->addObject( "bottle", morphingShader );

  // Load model from file.
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_high.obj" );
  ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_high.mtl" );


  bottle->genMorphTarget();
  Object *bottleMorphTarget = bottle->morphTarget();
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle_liquor_high.obj" );
  ObjLoader::loadMaterialFromFile( bottleMorphTarget, "../models/bottle_liquor_high.mtl" );

  //Morphing Items
  //Scale source and destination height to unit 0-1
  int heightScale = 10;
  int widthScale = 1;
  int depthScale = 1;
  ScaleModel * scaleModel = new ScaleModel(bottle, bottleMorphTarget,widthScale,heightScale,depthScale);
  rectangularMapping(bottle,bottleMorphTarget);
  //Rescale models to original size
  scaleModel->restoreModels();


  // Scale the bottle down!
  //bottle->_trans._scale.set( 0.30 );
  vec4 min = bottle->getMin();
  bottle->_trans._offset.set( 0, (0 - min.y), 0 );
  bottle->buffer();
  bottle->propagateOLD();

  // this obscure allusion to "the thong song" brought to you by Eric McCann
  GLuint sisqo = glGetUniformLocation( bottle->shader(),
				       "letMeSeeThatPhong" );
  glUniform1f( sisqo, true );
  
  // Let the bodies hit the table
  Object *table;
  table = rootScene->addObject( "table", noMorphShader );
  ObjLoader::loadModelFromFile(table, "../models/table_tx.obj");
  ObjLoader::loadMaterialFromFile(table, "../models/table_tx.mtl");
  table->texture("../Textures/texture_wood.png");
  table->buffer();

  fprintf(stderr, "table! (%f, %f, %f)\n", table->getMax().x, table->getMax().y, table->getMax().z);

  // Load up that goddamned candle
  Object *stick;
  Object *candle_top;
  Object *candle_top_melted;
  Object *candle_base;

  candle_top = rootScene->addObject( "candle_top", noMorphShader );
  candle_base = rootScene->addObject("candle_base", noMorphShader );
  stick = rootScene->addObject("stick", noMorphShader );

  ObjLoader::loadModelFromFile( candle_top, "../models/candle_top_unmelted.obj" );
  ObjLoader::loadMaterialFromFile( candle_top, "../models/candle.mtl" );
  ObjLoader::loadModelFromFile(candle_base, "../models/candle_bottom.obj");
  ObjLoader::loadMaterialFromFile( candle_base, "../models/candle.mtl" );
  ObjLoader::loadModelFromFile(stick, "../models/candlestick.obj");
  ObjLoader::loadMaterialFromFile(stick, "../models/candlestick.mtl");

/*
  candle_top->genMorphTarget();
  candle_top_melted = candle_top->morphTarget();
  ObjLoader::loadModelFromFile(candle_top_melted, "../models/candle_top_melted.obj");
  ObjLoader::loadMaterialFromFile(candle_top_melted, "../models/candle.mtl");
  */

  min = candle_base->getMin();
  vec4 max = stick->getMax();
  fprintf( stderr, "Min: (%f,%f,%f)\nMax: (%f,%f,%f)\n", min.x, min.y, min.z,
           max.x, max.y, max.z );
  candle_top->_trans._offset.set( 2.5, max.y - 4, 2.5 );
  candle_base->_trans._offset.set( 2.5, max.y - 4, 2.5 );
  stick->_trans._offset.set( 2.5, 0, 2.5);

  candle_top->propagateOLD();
  candle_base->propagateOLD();
  stick->propagateOLD();

  candle_top->buffer();
  candle_base->buffer();
  stick->buffer();

  stick->insertObject(candle_top);
  stick->insertObject(candle_base);

  
  max = candle_top->getMax();
  ps = new ParticleSystem( 1000, "ps1", particleShader );
  ps->setLifespan( 7.0, 12.0 );
  ps->setVectorField( ParticleFieldFunctions::flame);
  ps->setColorFunc(   ColorFunctions::flame );
  ps->setEmitterRadius( 0.02 );
  candle_top->insertObject( ps );
  ps->_trans._offset.set( 0, max.y, 0 );

 /* If you fill the system, the flame will have a non-flamelike pulsing effect. 
    Please don't!
 */
  //ps->fillSystemWithParticles(); 

  //ps->propagateOLD();
  candle_top->propagateOLD();

  Engine::instance()->cams()->active()->pos(2.0, 5.0, 9.0);
  
#ifndef WITHOUT_QT
#ifndef __APPLE__
  printf("LOOPING!\n");
  glutMainLoop(); // on Linux the glutMaintLoop is called here when using Qt
#endif
#else
  printf("LOOPING!\n");
  glutMainLoop(); // if we are not using Qt, the glutMainLoop is called here for both platforms.
#endif
}
/**
 * A simple animation callback.
 * Rotates the object about its Y axis,
 * as it orbits the object around a point about the Y axis.
 * @param obj The object to animate.
 */
void MONOLITH::simpleRotateAnim( TransCache &obj )
{
//    obj._rotation.rotateY( tick.scale() * 1.5 );
//    //obj._offset.set( 1.5, 0, 0 );
//    obj._orbit.rotateY( tick.scale() * -0.5 );
}

void MONOLITH::candleMeltAnim(TransCache &obj) {
  //obj._offset.set(0.0, 0.0, 0.0);
  obj._scale.adjust(1.0, 0.999, 1.0);
}

void MONOLITH::candleTopMeltDown(TransCache &obj) {
 // obj._offset.delta(0.0, -0.0025, 0.0);
}

/**
 * SHOW ENGINE WHERE OUR GOAT IS //ralphy may allusion
 */
void MONOLITH::raytraceStatusChanged(bool newstatus)
{
  if (Engine::instance()->glslVersion() < 1.50)
  {
    gprint(PRINT_ERROR, "Your hardware is tired, brah.\n");
    return;
  }
  static RayTracer rt;
  if (newstatus)
  {
    //TODO handle particles
    //TODO handle scene graph
    printf("SWITCHING TO RAY TRACING SHADER!\n");
    Engine::instance()->registerDisplayFunc(rt.display);
    rt.init(shader[3]);
    std::vector<Object *> objs;
    //ITERATE OVER ALL OBJS IN SCENE!
    rt.genereateScene(objs);
  }
  else
  {
    printf("SWITCHING TO NORMAL SHADER!\n");
    Engine::instance()->unregisterDisplayFunc();
  }
  printf("TODO: SWITCH VERTICES AND PUSH STUFF TO GPU APPROPRIATELY!\n");
}

