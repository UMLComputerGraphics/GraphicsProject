/**
 @file MONOLITH.cpp
 @author UMLComputerGraphics <https://github.com/UMLComputerGraphics>
 @date 2013-03-29
 @brief This is a monolith of every component.
 @details Final project rough draft.
 Original engine based on Ed Angel's book code.
 **/

#include "MONOLITH.hpp"


using soundHelper::result;

MONOLITH::~MONOLITH(void)
{
  cleanup();
}

/* Default and only constructor */
MONOLITH::MONOLITH(int argc, char** argv) :
   extinguish(false),
   flicker(false),
   ps(NULL),
   _defaultNumberOfParticles(3000)
{
    //As this happens before run() initializes relative paths, we need to initialize relative paths here
    Util::InitRelativePaths(argc, argv);

    /* sound stuff */
    soundHelper::fModInit( &(this->fSystem) ) ; // init the system

    soundHelper::add3dSound("../sounds/ForeverEndless_radio.wav",
              this->fSystem,
              &(this->foreverEndless),
              true); // add a sound to the system


    _l1 = _l2 = NULL;
    _argc = argc;
    _argv = argv;
    _VRMoveLeft = false;
    _VRMoveRight = false;
    _VRMoveUp = false;
    _VRMoveDown = false;
    _VRMoving = false;
}

/**
 * Cleans up our scene graph.
 */
void MONOLITH::cleanup(void) {
    extinguish = true;
    zipo.join();

    foreverEndless->release();
    fSystem->close();
    fSystem->release();
    
    if (_l1) delete _l1;
    if (_l2) delete _l2;
}

bool heisenbergUncertaintyPrinciple;
double morphTime = 0, prevTime = 0;
/**
 * Apply animations and whatever else your heart desires.
 */
void MONOLITH::monolith_idle(void)
{
#ifndef __APPLE__
  rt.idleHandsSpendTimeWithTheTextureBuffer();
#endif
  static Scene *rootScene = Engine::instance()->rootScene();
  Object *bottle = rootScene->search("bottle");

    // Animation variables.
    double timer = glutGet( GLUT_ELAPSED_TIME ) / 500.0;

    if (bottle && (bottle->morphEnabled())) {
	    morphTime += timer-prevTime;
    }
    prevTime = timer;

    float percent = (-cos( morphTime ) + 1.0) / 2.0;

    // Candle-melt Animation.
    {
      Object *candle = rootScene->search( "candle" );
      Object *candletip = rootScene->search( "candletip" );

      if (candle && candletip) {
#ifndef WITHOUT_QT
          if( candle->getRealMax().y - candle->getRealMin().y <= .15 ) sigEnableParticlesMelted( false );
#endif
          if( ps->getEnableTheSystem() && (ps->getNumParticlesActual() >= 500) ){
              Animation::candleMelt( candle, candletip, 0.9995 );
          }
      }
    }

    if(_VRCameraControl){
        vrlook(Engine::instance()->currentCamera(),_VRCameraCoordinates,Angel::vec2(1,1));
    }

    // Update the morph percentage.
    if (bottle && (bottle->morphEnabled())) {
      bottle->morphPercentage(percent);

#ifndef WITHOUT_QT
      int pct = (int)floor(percent * 100.0);
      if (_percentageCallback) {
    //prevents the slider event from being handled when the slider's value is set programatically.
    //  (the position and velocity of a subatomic particle can't be known simultaneously)
    heisenbergUncertaintyPrinciple = true;
    _percentageCallback(pct);
    heisenbergUncertaintyPrinciple = false;
      }
#endif
    }

    soundHelper::updateListener(  Engine::instance()->mainScreen()->_camList.active(),
                  this->fSystem);

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

void MONOLITH::slotEnableVRCameraControl(bool isEnabled){
    _VRCameraControl = isEnabled;
}

void MONOLITH::slotVRCameraCoordinates(float x, float y){
    _VRCameraCoordinates = Angel::vec2(x,y);
}

void MONOLITH::slotMorphPercentage(int value)
{
  if (!heisenbergUncertaintyPrinciple){
    rootScene->search("bottle")->morphPercentage(value / 100.0);
    _percentageCallback(value);
  }
}
void MONOLITH::setMorphPercentageCallback(boost::function<void(int)> cb)
{
    _percentageCallback = cb;
}
void MONOLITH::slotEnableMorphing(bool isEnabled)
{
  gprint(PRINT_WARNING, "MORPHING := %s\n", isEnabled?"ENABLED":"DISABLED");
   rootScene->search("bottle")->morphEnabled(isEnabled);
}

void MONOLITH::slotMorphToWineBottle(void)
{
   rootScene->search("bottle")->morphPercentage(0.0);
    _percentageCallback(0);
}

void MONOLITH::slotMorphToWhiskyBottle(void)
{
    rootScene->search("bottle")->morphPercentage(1.0);
    _percentageCallback(100);
}

void MONOLITH::slotEnableMorphMatching(bool isEnabled){
    if(!_morphMatchCalculated){
        int heightScale = 10;
        int widthScale = 1;
        int depthScale = 1;
        _scaleModel = new ScaleModel(bottle, bottle->morphTarget(),widthScale,heightScale,depthScale);
        _rectangularMapping = new RectangularMapping(bottle,bottle->morphTarget());
        _scaleModel->restoreModels();
        _morphMatchCalculated = true;
    }
    gprint(PRINT_WARNING, "MORPH MATCHING := %s\n", isEnabled?"ENABLED":"DISABLED");
    if(isEnabled){
        _rectangularMapping->copyToObjects(rootScene->search("bottle"),rootScene->search("bottle")->morphTarget());
        _scaleModel->restoreModels();
    }else{
        _rectangularMapping->revertToOriginal(rootScene->search("bottle"),rootScene->search("bottle")->morphTarget());
    }
    rootScene->search("bottle")->buffer();
}


void MONOLITH::slotEnableRaytracing(bool enabled)
{
   Engine::instance()->setRaytrace(enabled);
}

void MONOLITH::slotEnableParticleSystem(bool isEnabled)
{
    if (isEnabled)
    {
        ps->setEnableTheSystem( true );
        ps->setNumParticles(1000);
//        Engine::instance()->getLights()->at(0)->intensity(1);
//        Engine::instance()->setLights();
//        flicker = true;
    }
    else
    {
        ps->setNumParticles(0);
        ps->setEnableTheSystem( false );
//        flicker = false;
//        Engine::instance()->getLights()->at(0)->intensity(0);
//        Engine::instance()->setLights();
    }
}

void MONOLITH::slotUpdateFlameVecFunc(float pos[3], double scale, float power, float range)
{
    ps->setFuncParams(new FlameParameters( vec3(pos[0], pos[1], pos[2]), scale, power, range));
    ps->setVectorField( ParticleFieldFunctions::flame );
}

void MONOLITH::slotUpdateFlameVecFunc()
{
    ps->setFuncParams(new FlameParameters() );
    ps->setVectorField( ParticleFieldFunctions::flame );
}

void MONOLITH::slotUpdateTornadoVecFunc( float a, float b, float c )
{
    ps->setFuncParams( new TornadoParameters( a, b, c ));
    ps->setVectorField( ParticleFieldFunctions::tornado );
}

void MONOLITH::slotUpdateTornadoVecFunc()
{
    ps->setFuncParams( new TornadoParameters() );
    ps->setVectorField( ParticleFieldFunctions::tornado );
}

void MONOLITH::slotSetParticleLife( float min, float max )
{
    ps->setLifespan( min, max );
    ps->setRespawnFlag( true );
}

void MONOLITH::slotUpdateVectorField(std::string fx, std::string fy, std::string fz)
{
  sigUpdateUdfMessage( "" );
  try {
    UserVectorField *temp = new UserVectorField( fx, fy, fz );
    // Note, temp's memory is now managed by the particlesystem.
    // The previous UVF is also now freed as-of the switch.
    ps->switchUVF( temp );
    ps->setFuncParams( new UserParameters(ps->uvf()) );
    ps->setVectorField( ParticleFieldFunctions::userSupplied );
  }
  catch( std::exception &e ) {
    sigUpdateUdfMessage( "Could not compute user function" );
    gprint( PRINT_WARNING, "Could not enable the requested user function, because: %s\n", e.what() );
  }

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

void MONOLITH::slotPauseMusic(bool isPaused)
{ // PAUSED => true  ::  UNPAUSED => false
    radio->setPaused(isPaused);
    soundHelper::ERRCHECK();
}

void MONOLITH::slotMusicVolume(int vol)
{ // 0.0 => silent :: 1.0 => MAX VOLUME
    float newVol = vol / 100.0 ;

    radio->setVolume(newVol);
    soundHelper::ERRCHECK();
}

void MONOLITH::slotPartColorFunc( int index )
{
    switch ( index )
    {
    case 0:
        ps->setColorFunc( ColorFunctions::flame );
        break;
    case 1:
        ps->setColorFunc( ColorFunctions::aurora );
        break;
    case 2:
        ps->setColorFunc( ColorFunctions::HSV );
        break;
    case 3:
        ps->setColorFunc( ColorFunctions::tropical );
        break;
    case 4:
        ps->setColorFunc( ColorFunctions::galaxy );
        break;
    }
}

#endif //WITHOUT_QT

/**
 * This will initialize and run MONOLITH
 */
void MONOLITH::run() {
  Engine::init( &_argc, _argv, "Graphics II Spring 2013 Final Project" );
  Engine *eng = Engine::instance();

  _l1 = new Light( "CandleLight", 2.5, 6.8, 2.5 );
  _l1->color(vec3(1.0, 0.5, 0.2));
  Engine::instance()->addLight(_l1);
  _l2 = new Light( "Scene Light", 0, 18, 0 );
  _l2->intensity(28);
  Engine::instance()->addLight(_l2);

#ifndef WITHOUT_QT
  _l1->intensity(0);
  flicker = false;
#else
  flicker = true;
#endif

  // Now that we have lights, let's make the thread, rather than hoping it will magically re-enter the thread function it broke out of.
  zipo = boost::thread(boost::bind(&MONOLITH::aRomanticEvening, this));

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

  // ============ Object Placement / Scene Generation ============


  // ************ Table ************
  Object *table = rootScene->addObject( "table", noMorphShader );
  ObjLoader::loadModelFromFile(table, "../models/table_tx.obj");
  ObjLoader::loadMaterialFromFile(table, "../models/table_tx.mtl");
  table->texture("../Textures/texture_wood.png");
  Animation::seekTopTo( table, 0 );
  table->buffer();
  glUniform1i( glGetUniformLocation( table->shader(), "letMeSeeThatPhong" ), 1 );


  // ************ Wine Bottle ************
  bottle = table->addObject( "bottle", morphingShader );
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_high.obj" );
  ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_high.mtl" );
  Animation::seekBottomTo( bottle, 0.001 );

  bottle->genMorphTarget();
  Object *bottleMorphTarget = bottle->morphTarget();
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle_liquor_high3.obj" );
  ObjLoader::loadMaterialFromFile( bottleMorphTarget, "../models/bottle_liquor_high.mtl" );
  Animation::seekBottomTo( bottleMorphTarget, 0.001 );

  static const bool useZachMorphing = false;
  if (useZachMorphing) {
    int heightScale = 10;
    int widthScale = 1;
    int depthScale = 1;
    _scaleModel = new ScaleModel(bottle, bottleMorphTarget,widthScale,heightScale,depthScale);
    _rectangularMapping = new RectangularMapping(bottle,bottleMorphTarget);
    _scaleModel->restoreModels();
    _morphMatchCalculated = true;
  } else {
    _morphMatchCalculated = false;
  }
  bottle->buffer();
  glUniform1i( glGetUniformLocation( bottle->shader(), "letMeSeeThatPhong" ), 1 );

  // ************ Radio ************
  Object *radio = table->addObject( "radio", noMorphShader );
  ObjLoader::loadModelFromFile( radio, "../models/radio-tx.obj" );
  ObjLoader::loadMaterialFromFile( radio, "../models/radio-tx.mtl" );
  radio->texture("../Textures/texture_radio.png");
  Animation::seekBottomTo( radio, 0.001 );
  radio->_trans.push( RotMat( 0.0, -45.0, 0.0 ) );
  Animation::seekCenterTo( radio, 9.0, -7.0 );
  radio->buffer();
  glUniform1i( glGetUniformLocation( radio->shader(), "letMeSeeThatPhong" ), 1 );


  // ************ Candlestick ************
  Object *candlestick = table->addObject( "candlestick", noMorphShader );
  ObjLoader::loadModelFromFile(candlestick, "../models/candlestick.obj");
  ObjLoader::loadMaterialFromFile(candlestick, "../models/candlestick.mtl");
  Animation::seekBottomTo( candlestick, 0.001 );
  Animation::seekCenterTo( candlestick, 2.5, 2.5, true );
  candlestick->buffer();
  glUniform1i( glGetUniformLocation( candlestick->shader(), "letMeSeeThatPhong" ), 1 );


  // ************ Candle ************
  Object *candle = candlestick->addObject( "candle", noMorphShader );
  ObjLoader::loadModelFromFile( candle, "../models/candle_bottom.obj");
  ObjLoader::loadMaterialFromFile( candle, "../models/candle.mtl" );
  candle->buffer();
  glUniform1i( glGetUniformLocation( candle->shader(), "letMeSeeThatPhong" ), 1 );
  

  // ************ Candletip ************
  Object *candletip = candle->addObject( "candletip", noMorphShader );
  ObjLoader::loadModelFromFile( candletip, "../models/candle_top_unmelted.obj" );
  ObjLoader::loadMaterialFromFile( candletip, "../models/candle.mtl" );
  candletip->buffer();
  glUniform1i( glGetUniformLocation( candletip->shader(), "letMeSeeThatPhong" ), 1 );


  // ************ Particle System ************
#ifndef WITHOUT_QT
  ps = new ParticleSystem(0, "ps1", particleShader);
#else
  ps = new ParticleSystem( _defaultNumberOfParticles, "ps1", particleShader );
#endif
  ps->setLifespan( 9.0, 12.0 );
  ps->setVectorField( ParticleFieldFunctions::flame );
  ps->setColorFunc( ColorFunctions::flame );
  ps->setEmitterRadius( 0.05 );
  candletip->insertObject( ps );
  ps->_trans.push( ScaleMat( 1.5 ) );
  /** We're going to move the flame roughly 75% into the candletip. **/
  float heightAdj = (0.25) * (candletip->getMax().y - candletip->getMin().y);
  ps->_trans.push( TransMat( 0, candletip->getMax().y - heightAdj, 0 ) );
  ps->setEmitterShape(PS_HEMI_D);
  glPointSize( 1.8 );


  Engine::instance()->cams()->active()->pos(2.0, 5.0, 9.0);

  //Set lights for all objects in the scene
  Engine::instance()->setLights();

  // need this for smoothness
  glShadeModel(GL_SMOOTH);

  soundHelper::play3dSound( vec4(-9.0,2.0,7.0,1.0),
                vec4(0.0,0.0,0.0,1.0),
                this->fSystem,
                &(this->radio),
                this->foreverEndless);

  glUniform1i( glGetUniformLocation( morphingShader, "letMeSeeThatPhong" ), 1 );
  glUniform1i( glGetUniformLocation( noMorphShader, "letMeSeeThatPhong" ), 1 );

#ifndef WITHOUT_QT
#ifndef __APPLE__
  // on Linux the glutMaintLoop is called here when using Qt
  Engine::run();
  //glutMainLoop();
#endif
#else
  // if we are not using Qt, the glutMainLoop is called here for both platforms.
  Engine::run();
  //glutMainLoop();
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
  if (newstatus)
  {
    //TODO handle particles
    //TODO handle scene graph
    printf("SWITCHING TO RAY TRACING SHADER!\n");
    Engine::instance()->registerDisplayFunc(rt.display);
    rt.init(shader[3]);
    rt.pushDataToBuffer();
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
  printf("STARTING ROMANCE!\n");
  while ( !extinguish ) {

    // random number between 0 and 1
    float lightness = (float) rand() / (float) RAND_MAX;
    // between 0 and .3
    lightness = lightness * 3.0 / 10.0;
    lightness += .7;

    if (ps) lightness *= (ps->getNumParticlesVisible() / 3000.0);
    //lightness = (float)std::max(0.0,std::min((double)lightness,1.0));

    // This is dumb and it's why singletons are stupid,
    // but if the engine has exited before we can extinguish this,
    // we need to not re-recreate the engine :P
    if (Engine::exists()) {
      Engine::instance()->safeSetIntensity(0, lightness);
      Engine::instance()->setLights();
    }

    boost::this_thread::yield();
    sleep( 0.01 );
  }
  printf("ENDING ROMANCE!\n");
}

/**
 VRlook is an analog of mouselook, for VR location controls.
 It takes a reference to a Camera, and two vec2s,
 and uses the information to adjust the Camera's _rotation.

 @param VRCamera The camera to adjust the _rotation of.
 @param NewTheta  The X,Y coordinates of the VR contorls.
 @param MovementRates The X, Y angular velocities of the VR Controls.

 @return Void.

**/
void MONOLITH::vrlook( Camera *VRCamera, const Angel::vec2 &NewTheta,
              const Angel::vec2 &MovementRates ){

    if((NewTheta.x > 1/3.0*280.0)&&(NewTheta.x < 2/3.0*280.0)&&(NewTheta.y < 2/3.0*157.0)&&(NewTheta.y >1/3.0*157.0)&&(NewTheta.x != 0.0)&&(NewTheta.y != 0.0)){
        if(_VRMoveLeft){
            //printf("VR Stop Left\n");
          VRCamera->stop( Camera::DIR_LEFT );
          _VRMoveLeft = false;
        }else if (_VRMoveRight){
            //printf("VR Stop Right\n");
          VRCamera->stop( Camera::DIR_RIGHT);
          _VRMoveRight = false;
        }

        if(_VRMoveUp){
            VRCamera->stop( Camera::DIR_UP);
            //printf("VR Stop Up\n");
            _VRMoveUp = false;
        }else if (_VRMoveDown){
            VRCamera->stop( Camera::DIR_DOWN);
            //printf("VR Stop Down\n");
            _VRMoveDown = false;
        }

    }else if((NewTheta.x != 0.0)&&(NewTheta.y != 0.0)){
      if(NewTheta.x < 1/3.0*280.0){
          //printf("VR Move Left: %f < %f\n",NewTheta.x,1/3.0*280.0);
            _VRMoveLeft = true;
            VRCamera->move( Camera::DIR_LEFT );
      }else if(NewTheta.x > 2/3.0*280.0){
          //printf("VR Move Right\n");
            VRCamera->move( Camera::DIR_RIGHT );
            _VRMoveRight = true;
      }

      if(NewTheta.y > 2/3.0*157.0){
          //printf("VR Move Up\n");
        VRCamera->move( Camera::DIR_UP );
        _VRMoveUp = true;
      }else if(NewTheta.y < 1/3.0*157.0){
          //printf("VR Move Down\n");
        VRCamera->move( Camera::DIR_DOWN );
        _VRMoveDown = true;
      }
    }
}
