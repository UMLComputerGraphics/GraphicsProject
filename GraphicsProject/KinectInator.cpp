/**
 * @file KinectInator.cpp
 * @date 2013-03-13
 * @authors PrimeSense Ltd, Eric McCann
 * @brief FIXME: Documentation needed from Eric.
 */

/**************************************************
 *  ERIC MCCANN WAS HERE AND TURNED THIS INSIDE OUT.
 *  IT WAS ORIGINALY NiSimpleSkeleton.cpp FROM PRIMESENSE SAMPLES
 *  ORIGINAL LICENSE BLOCK STUCK AT TOP OF KinectInator.h
 *  now it's abstracted in a class, and runs in parallel with the opengl mainloop
 **************************************************/

//#define EDBG
#include "KinectInator.hpp"

//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

XnBool fileExists( const char *fn ) {
  XnBool exists;
  xnOSDoesFileExist( fn, &exists );
  return exists;
}

namespace TiemSpelchk {
// Callback: New user was detected
  void XN_CALLBACK_TYPE Lurn2SpielNub::User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
  { 
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d New User %d\n", epochTime, nId);
    // New user found
    if (g_bNeedPose)
    { 
      g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
    }
    else
    { 
      g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
  }
// Callback: An existing user was lost
  void XN_CALLBACK_TYPE Lurn2SpielNub::User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
  { 
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Lost user %d\n", epochTime, nId);
  }
// Callback: Detected a pose
  void XN_CALLBACK_TYPE Lurn2SpielNub::UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
  { 
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
  }
// Callback: Started calibration
  void XN_CALLBACK_TYPE Lurn2SpielNub::UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
  { 
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Calibration started for user %d\n", epochTime, nId);
  }

  void XN_CALLBACK_TYPE Lurn2SpielNub::UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
  { 
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK)
    { 
      // Calibration succeeded
      printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);
      g_UserGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    { 
      // Calibration failed
      printf("%d Calibration failed for user %d\n", epochTime, nId);
      if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
      { 
        printf("Manual abort occured, stop attempting to calibrate!");
        return;
      }
      if (g_bNeedPose)
      { 
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
      }
      else
      { 
        g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
      }
    }
  }

#define CHECK_RC(nRetVal, what)					    \
    if (nRetVal != XN_STATUS_OK)				    \
{								    \
    printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));    \
    return nRetVal;						    \
}
  
  void Lurn2SpielNub::FUNKMASTER_thread_func() {
    XnUserID aUsers[MAX_NUM_USERS];
    XnUInt16 nUsers;
    XnSkeletonJointTransformation torsoJoint;
    
    printf( "KICK THE TIRES AND LIGHT THE FIRES\n" );
    //now print some annoying ascii art
    printf(
        "-----------------------------------------\n-------------------\n\\\\===\\\\   || //=\\\\\n \\\\   \\\\  ||//===\\\\\n //   //  ||\\\\\n//   //===|| \\\\===\n-------------------\n-----------------------------------------" );
    if ( g_bNeedPose ) {
      printf( "\n\n\n\nSTICK 'EM UP, SCUMBAG!\n" );
      printf( "(Assume the \"Psi\" position to callibrate)" );
    }
    while ( !needsToSeppuku ) {
      g_Context.WaitOneUpdateAll( g_UserGenerator );//this blocks, so we want to abort if it returns after shutdown is called
      if ( needsToSeppuku ) break;
      
      //call our callback function for whichever user we're currently tracking        
      nUsers = MAX_NUM_USERS;
      g_UserGenerator.GetUsers( aUsers, nUsers );
      float minz = FLT_MAX, x, y;
      int u = -1;
      for ( XnUInt16 i = 0; i < nUsers; i++ ) {
        if ( g_UserGenerator.GetSkeletonCap().IsTracking( aUsers[i] ) == FALSE )
          continue;
        
        g_UserGenerator.GetSkeletonCap().GetSkeletonJoint( aUsers[i],
                                                           XN_SKEL_TORSO,
                                                           torsoJoint );
        
        //TODO: ONLY SEND CALLBACK ONCE FOR CLOSEST USER IN LIST!!!
        
#ifdef EDBG
        printhead(aUsers[i], torsoJoint.position.position.X, torsoJoint.position.position.Y, torsoJoint.position.position.Z);
#endif
        if ( torsoJoint.position.position.Z < minz ) {
          x = torsoJoint.position.position.X;
          y = torsoJoint.position.position.Y;
          minz = torsoJoint.position.position.Z;
          u = aUsers[i];
        }
      }
      if ( u != -1 ) _cb( u, x, y, minz );
    }
  }
  
  Lurn2SpielNub::Lurn2SpielNub() {
    g_bNeedPose = false;
    needsToSeppuku = true;
    //currenthead = -1;
    //prevent double-printing when instantiated with default constructor, and debug is enabled
#ifdef EDBG
    _cb = &noop;
#else
    _cb = &printhead;
#endif
  }
  
  void Lurn2SpielNub::setCallback(
      boost::function< void( int, double, double, double ) > CB ) {
    _cb = CB;
  }
  
  boost::function<void XN_CALLBACK_TYPE (xn::UserGenerator&, XnUserID, void*)> _new_user;
  boost::function<void XN_CALLBACK_TYPE (xn::UserGenerator&, XnUserID, void*)> _lost_user;
  boost::function<void XN_CALLBACK_TYPE (xn::PoseDetectionCapability&, const XnChar*, XnUserID, void*)> _pose;
  boost::function<void XN_CALLBACK_TYPE (xn::SkeletonCapability&, XnUserID, void*)> _cal_start;
  boost::function<void XN_CALLBACK_TYPE (xn::SkeletonCapability&, XnUserID, XnCalibrationStatus, void*)> _cal_complete;
  void Lurn2SpielNub::new_user( xn::UserGenerator& a, XnUserID b, void* c ) {
    _new_user( a, b, c );
  }
  void Lurn2SpielNub::lost_user( xn::UserGenerator& a, XnUserID b, void* c ) {
    _lost_user( a, b, c );
  }
  void Lurn2SpielNub::pose( xn::PoseDetectionCapability& a, const XnChar* b,
                            XnUserID c, void* d ) {
    _pose( a, b, c, d );
  }
  void Lurn2SpielNub::cal_start( xn::SkeletonCapability& a, XnUserID b,
                                 void* c ) {
    _cal_start( a, b, c );
  }
  void Lurn2SpielNub::cal_complete( xn::SkeletonCapability& a, XnUserID b,
                                    XnCalibrationStatus c, void* d ) {
    _cal_complete( a, b, c, d );
  }
  
  Lurn2SpielNub::~Lurn2SpielNub() {
    Shutdown();
  }
  
  void Lurn2SpielNub::Shutdown() {
    needsToSeppuku = true;
  }
  
  int Lurn2SpielNub::Start() {
    if ( needsToSeppuku ) {
      needsToSeppuku = false;
      
      XnStatus nRetVal = XN_STATUS_OK;
      xn::EnumerationErrors errors;
      
      const char *fn = NULL;
      if ( fileExists( SAMPLE_XML_PATH ) ) fn = SAMPLE_XML_PATH;
      else if ( fileExists( SAMPLE_XML_PATH_LOCAL ) ) fn = SAMPLE_XML_PATH_LOCAL;
      else {
        printf( "Could not find '%s' nor '%s'. Aborting.\n", SAMPLE_XML_PATH,
                SAMPLE_XML_PATH_LOCAL );
        return XN_STATUS_ERROR;
      }
      printf( "Reading config from: '%s'\n", fn );
      
      nRetVal = g_Context.InitFromXmlFile( fn, g_scriptNode, &errors );
      if ( nRetVal == XN_STATUS_NO_NODE_PRESENT ) {
        XnChar strError[1024];
        errors.ToString( strError, 1024 );
        printf( "%s\n", strError );
        return (nRetVal);
      } else if ( nRetVal != XN_STATUS_OK ) {
        printf( "Open failed: %s\n", xnGetStatusString( nRetVal ) );
        return (nRetVal);
      }
      
      nRetVal = g_Context.FindExistingNode( XN_NODE_TYPE_DEPTH,
                                            g_DepthGenerator );
      CHECK_RC( nRetVal, "No depth" );
      
      nRetVal = g_Context.FindExistingNode( XN_NODE_TYPE_USER,
                                            g_UserGenerator );
      if ( nRetVal != XN_STATUS_OK ) {
        nRetVal = g_UserGenerator.Create( g_Context );
        CHECK_RC( nRetVal, "Find user generator" );
      }
      
      XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete,
          hPoseDetected;
      if ( !g_UserGenerator.IsCapabilitySupported( XN_CAPABILITY_SKELETON ) ) {
        printf( "Supplied user generator doesn't support skeleton\n" );
        return 1;
      }
      _new_user = boost::bind( &Lurn2SpielNub::User_NewUser, this, _1, _2, _3 );
      _lost_user = boost::bind( &Lurn2SpielNub::User_LostUser, this, _1, _2,
                                _3 );
      _cal_start = boost::bind(
          &Lurn2SpielNub::UserCalibration_CalibrationStart, this, _1, _2, _3 );
      _cal_complete = boost::bind(
          &Lurn2SpielNub::UserCalibration_CalibrationComplete, this, _1, _2, _3,
          _4 );
      _pose = boost::bind( &Lurn2SpielNub::UserPose_PoseDetected, this, _1, _2,
                           _3, _4 );
      
      nRetVal = g_UserGenerator.RegisterUserCallbacks(
          &Lurn2SpielNub::new_user, &Lurn2SpielNub::lost_user, NULL,
          hUserCallbacks );
      CHECK_RC( nRetVal, "Register to user callbacks" );
      nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(
          &Lurn2SpielNub::cal_start, NULL, hCalibrationStart );
      CHECK_RC( nRetVal, "Register to calibration start" );
      nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(
          &Lurn2SpielNub::cal_complete, NULL, hCalibrationComplete );
      CHECK_RC( nRetVal, "Register to calibration complete" );
      
      if ( g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration() ) {
        g_bNeedPose = TRUE;
        if ( !g_UserGenerator.IsCapabilitySupported(
            XN_CAPABILITY_POSE_DETECTION ) ) {
          printf( "Pose required, but not supported\n" );
          return 1;
        }
        nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(
            &Lurn2SpielNub::pose, NULL, hPoseDetected );
        CHECK_RC( nRetVal, "Register to Pose Detected" );
        g_UserGenerator.GetSkeletonCap().GetCalibrationPose( g_strPose );
      }
      
      g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(
          XN_SKEL_PROFILE_ALL );
      
      nRetVal = g_Context.StartGeneratingAll();
      CHECK_RC( nRetVal, "StartGenerating" );
      
      _thread = boost::thread( &Lurn2SpielNub::FUNKMASTER_thread_func, this );
    }
    return 0;
  }
}

void printhead( int user, double x, double y, double z ) {
  printf( "user %d: head at (%6.2f,%6.2f,%6.2f)\n", user, x, y, z );
}
void noop( int user, double x, double y, double z ) {
}
