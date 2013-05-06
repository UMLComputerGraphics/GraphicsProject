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
  cleanup();
}

/* Default and only constructor */
MONOLITH::MONOLITH(int argc, char** argv) :
    extinguish( false ),
    _defaultNumberOfParticles(3000)
    /*    zipo(boost::thread(boost::bind(&MONOLITH::aRomanticEvening, this))) */
{
  _argc = argc;
  _argv = argv;

    Light* l = new Light( "CandleLight", 2.5, 6.8, 2.5 );
    l->color(vec3(1.0, 0.5, 0.2));
    Engine::instance()->addLight(l);
    Light *l2 = new Light( "Scene Light", 0, 18, 0 );
    l2->intensity(28);
    Engine::instance()->addLight(l2);

}

/**
 * Cleans up our scene graph.
 */
void MONOLITH::cleanup(void) {
    extinguish = true;
    //zipo.join();
}

bool heisenbergUncertaintyPrinciple;
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

#ifndef WITHOUT_QT
        int pct = (int)floor(percent * 100.0);
        if (_percentageCallback)
        {
		// ????
          heisenbergUncertaintyPrinciple = true;
          _percentageCallback(pct);
          heisenbergUncertaintyPrinciple = false;
        }
#endif
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
    //printf("Particle system now has %d particles.\n", ps->getNumParticles());
}

void MONOLITH::slotFreezeParticles(bool isEnabled)
{
	ps->setPause(isEnabled);
}
void MONOLITH::slotMorphPercentage(int value)
{
  if (!heisenbergUncertaintyPrinciple)
    (*rootScene)["bottle"]->morphPercentage(value / 100.0);
}
void MONOLITH::setMorphPercentageCallback(boost::function<void(int)> cb)
{
    _percentageCallback = cb;
}
void MONOLITH::slotEnableMorphing(bool isEnabled)
{
  gprint(PRINT_WARNING, "MORPHING := %s\n", isEnabled?"ENABLED":"DISABLED");
   (*rootScene)["bottle"]->morphEnabled(isEnabled);
}

void MONOLITH::slotMorphToWineBottle(void)
{
   (*rootScene)["bottle"]->morphPercentage(0.0);
    _percentageCallback(0);
}

void MONOLITH::slotMorphToWhiskyBottle(void)
{
    (*rootScene)["bottle"]->morphPercentage(1.0);
    _percentageCallback(100);
}

void MONOLITH::slotEnableMorphMatching(bool isEnabled){
	gprint(PRINT_WARNING, "MORPH MATCHING := %s\n", isEnabled?"ENABLED":"DISABLED");
    if(isEnabled){
        _rectangularMapping->copyToObjects((*rootScene)["bottle"],(*rootScene)["bottle"]->morphTarget());
        _scaleModel->restoreModels();
    }else{
        _rectangularMapping->revertToOriginal((*rootScene)["bottle"],(*rootScene)["bottle"]->morphTarget());
    }
   	(*rootScene)["bottle"]->buffer();
}


void MONOLITH::slotEnableRaytracing(bool enabled)
{
   Engine::instance()->setRaytrace(enabled);
}

void MONOLITH::slotEnableParticleSystem(bool isEnabled)
{
    if (isEnabled)
    {
        ps->setNumParticles(_defaultNumberOfParticles);
    }
    else
    {
        ps->setNumParticles(0);
    }
}

void MONOLITH::slotUpdateFlameVecFunc(double pos[3], double scale, float power, float range)
{
    ps->setFuncParams(new FlameParameters( vec3(pos[0], pos[1], pos[2]), scale, power, range));
    ps->setVectorField( ParticleFieldFunctions::flame );
}

void MONOLITH::slotUpdateFlameVecFunc()
{
    ps->setFuncParams(new FlameParameters() );
    ps->setVectorField( ParticleFieldFunctions::flame );
}

/*
 * Deprecated... for now
void MONOLITH::slotParticleFieldFunction(int index)
{
    switch (index)
    {
    case 0:
        ps->setVectorField( ParticleFieldFunctions::flame);
        break;
    case 1:
        ps->setVectorField(ParticleFieldFunctions::tornado);
        break;
    case 2:

    default:
        ps->setVectorField( ParticleFieldFunctions::flame);
        break;
    }
}
*/

void MONOLITH::slotUpdateVectorField(std::string* params)
{
    Parameters* funcParams = new UserParameters(params);
    ps->setFuncParams(funcParams);
}

/**
 * @brief defaultNumberOfParticles (setter)
 * @param value
 */
void MONOLITH::defaultNumberOfParticles(int value)
{
    _defaultNumberOfParticles = value;
}

/**
 * @brief defaultNumberOfParticles (getter)
 * @return
 */
int MONOLITH::defaultNumberOfParticles()
{
    return _defaultNumberOfParticles;
}

void MONOLITH::slotMaxAcceleration(int num)
{
	Engine::instance()->currentCamera()->setMaxAcceleration((float) num);
}
void MONOLITH::slotFriction(int num)
{
	Engine::instance()->currentCamera()->setFriction((float) num);
}
void MONOLITH::slotSpeed(int num)
{
	Engine::instance()->currentCamera()->setSpeed((float) num);
}

void MONOLITH::slotMaxSpeed( int num )
{
    Engine::instance()->currentCamera()->setMaxSpeed((float) num) ;
}

void MONOLITH::slotCurrentView( int num )
{
    Engine::instance()->currentCamera()->setCurrentView(num) ;
}

#endif //WITHOUT_QT

/**
 * This will initialize and run MONOLITH
 */
void MONOLITH::run() {
  Engine::init( &_argc, _argv, "Graphics II Spring 2013 Final Project" );
  Engine *eng = Engine::instance();
  eng->registerIdle( boost::bind(&MONOLITH::monolith_idle, this) );
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
  //bottle->setLights(lightAmbient, &numLights, lightPositions, lightDiffuse, lightSpecular);

  bottle->genMorphTarget();
  Object *bottleMorphTarget = bottle->morphTarget();
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle_liquor_high3.obj" );
  ObjLoader::loadMaterialFromFile( bottleMorphTarget, "../models/bottle_liquor_high.mtl" );
  //bottleMorphTarget->setLights(lightAmbient, &numLights, lightPositions, lightDiffuse, lightSpecular);

  //Morphing Items
  //Scale source and destination height to unit 0-1
  static const bool useZachMorphing = false;
  if (useZachMorphing) {
    int heightScale = 10;
    int widthScale = 1;
    int depthScale = 1;
    _scaleModel = new ScaleModel(bottle, bottleMorphTarget,widthScale,heightScale,depthScale);
    _rectangularMapping = new RectangularMapping(bottle,bottleMorphTarget);
    //_rectangularMapping->copyToObjects(bottle,bottleMorphTarget);
    
    //Rescale models to original size
    _scaleModel->restoreModels();
  }

  // Scale the bottle down!
  //bottle->_trans._scale.set( 0.30 );
  vec4 min = bottle->getMin();
  bottle->_trans._offset.set( 0, (0 - min.y), 0 );
  bottle->buffer();
  bottle->propagateOLD();

  // this obscure allusion to "the thong song" brought to you by Eric McCann
  glUniform1i( glGetUniformLocation( bottle->shader(), "letMeSeeThatPhong" ), 1 );
  
  // Let the bodies hit the table
  Object *table;
  table = rootScene->addObject( "table", noMorphShader );
  ObjLoader::loadModelFromFile(table, "../models/table_tx.obj");
  ObjLoader::loadMaterialFromFile(table, "../models/table_tx.mtl");
  glUniform1i( glGetUniformLocation( table->shader(), "letMeSeeThatPhong" ), 1 );
  //table->setLights(lightAmbient, &numLights, lightPositions, lightDiffuse, lightSpecular);
  table->texture("../Textures/texture_wood.png");
  table->buffer();

  fprintf(stderr, "table! (%f, %f, %f)\n", table->getMax().x, table->getMax().y, table->getMax().z);

  // Load up that candle
  Object *stick;
  Object *candle_top;
  Object *candle_base;

  //candle_top = rootScene->addObject( "candle_top", morphingShader );
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
  Object *candle_top_melted = candle_top->morphTarget();
  ObjLoader::loadModelFromFile(candle_top_melted, "../models/candle_top_melted.obj");
  ObjLoader::loadMaterialFromFile(candle_top_melted, "../models/candle.mtl");
  */

  //candle_top->setLights(lightAmbient, &numLights, lightPositions, lightDiffuse, lightSpecular);
  //candle_base->setLights(lightAmbient, &numLights, lightPositions, lightDiffuse, lightSpecular);
  //stick->setLights(lightAmbient, &numLights, lightPositions, lightDiffuse, lightSpecular);

  glUniform1i(glGetUniformLocation(candle_top->shader(),"letMeSeeThatPhong"),1);
  glUniform1i(glGetUniformLocation(candle_base->shader(),"letMeSeeThatPhong"),1);
  glUniform1i(glGetUniformLocation(stick->shader(),"letMeSeeThatPhong"),1);
  //glUniform1i(glGetUniformLocation(candle_top_melted->shader(),"letMeSeeThatPhong"),1);

  min = candle_base->getMin();
  vec4 max = stick->getMax();
  fprintf( stderr, "Min: (%f,%f,%f)\nMax: (%f,%f,%f)\n", min.x, min.y, min.z,
           max.x, max.y, max.z );
  candle_top->_trans._offset.set( 2.5, max.y - 4.4, 2.5 );
  candle_base->_trans._offset.set( 2.5, max.y - 4.4, 2.5 );
  stick->_trans._offset.set( 2.5, 0, 2.5);

  /*
  ScaleModel * scaleModelCandle = new ScaleModel(candle_top, candle_top_melted,widthScale,heightScale,depthScale);
  RectangularMapping * rectangularMappingCandle = new RectangularMapping(candle_top, candle_top_melted);
  rectangularMappingCandle->copyToObjects(candle_top, candle_top_melted);
  rectangularMappingCandle->revertToOriginal(candle_top, candle_top_melted);
  scaleModelCandle->restoreModels();
  */

  candle_top->propagateOLD();
  candle_base->propagateOLD();
  stick->propagateOLD();

  candle_top->buffer();
  candle_base->buffer();
  stick->buffer();
  //candle_top_melted->buffer();

  stick->insertObject(candle_top);
  stick->insertObject(candle_base);

  max = candle_top->getMax();

  #ifndef WITHOUT_QT
  ps = new ParticleSystem(0, "ps1", particleShader);
  #else
  ps = new ParticleSystem( _defaultNumberOfParticles, "ps1", particleShader );
  #endif

  ps->setLifespan( 9.0, 12.0 );
  ps->setVectorField( ParticleFieldFunctions::flame);
  ps->setColorFunc(   ColorFunctions::flame );
  ps->setEmitterRadius( 0.05 );
  candle_top->insertObject( ps );
  ps->_trans._offset.set( 0, max.y - 0.02 , 0 );
  ps->_trans._scale.set( 1.5 );
  ps->setEmitterShape(PS_HEMI_D);

 /* If you fill the system, the flame will have a non-flamelike pulsing effect. 
    Please don't!
 */
  //ps->fillSystemWithParticles(); 

  //ps->propagateOLD();
  candle_top->propagateOLD();
  
  //Make the particles look bigger and awesomer
  glPointSize( 1.8 );

  Engine::instance()->cams()->active()->pos(2.0, 5.0, 9.0);
  
  //Set lights for all objects in the scene
  Engine::instance()->setLights();

  // need this for smoothness
  glShadeModel(GL_SMOOTH);
  
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
    gprint(PRINT_ERROR, "Raytracing is not supported on this system.\n");
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

    Engine::instance()->rootScene()->bufferToRaytracer( rt );
    rt.pushDataToBuffer();
    //    rt.genereateScene(objs);
  }
  else
  {
    printf("SWITCHING TO NORMAL SHADER!\n");
    Engine::instance()->unregisterDisplayFunc();
  }
  printf("TODO: SWITCH VERTICES AND PUSH STUFF TO GPU APPROPRIATELY!\n");
}

//flicker at constant rate, regardless of update loop
void MONOLITH::aRomanticEvening() {
  while ( !extinguish ) {
    // random number between 0 and 1
    float lightness = (float) rand() / (float) RAND_MAX;
    // between 0 and .3
    lightness = lightness * 3.0 / 10.0;

    lightness += .7;

    // @NVV: We can't assume here that lights exist yet,
    // This is throwing an exception for 0 being out-of-range.
    //Engine::instance()->getLights()->at(0)->intensity(lightness);
    //Engine::instance()->setLights();

    sleep( 0.01 );
  }
}
