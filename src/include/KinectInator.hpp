/**
 * @file KinectInator.hpp
 * @date 2013-03-13
 * @author PrimeSense Ltd.
 * @authors PrimeSense Ltd., Eric McCann
 * @brief FIXME: Documentation needed from Eric.
 */

/****************************************************************************
 *                                                                           *
 *  OpenNI 1.x Alpha                                                         *
 *  Copyright (C) 2011 PrimeSense Ltd.                                       *
 *                                                                           *
 *  This file is part of OpenNI.                                             *
 *                                                                           *
 *  OpenNI is free software: you can redistribute it and/or modify           *
 *  it under the terms of the GNU Lesser General Public License as published *
 *  by the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  OpenNI is distributed in the hope that it will be useful,                *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU Lesser General Public License *
 *  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                           *
 ****************************************************************************/

#ifndef __OPENNIFTW
/**************************************************
 *  ERIC MCCANN WAS HERE AND TURNED THIS INSIDE OUT.
 *  IT WAS ORIGINALY NiSimpleSkeleton.cpp FROM PRIMESENSE SAMPLES
 *  now it's abstracted in a class, and runs in parallel with the opengl mainloop
 **************************************************/
#define __OPENNIFTW
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnCppWrapper.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <cfloat>

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "OpenNIConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "OpenNIConfig.xml"
#define MAX_NUM_USERS 15

namespace TiemSpelchk {
  class Lurn2SpielNub {
  private:
    boost::function< void( int, double, double, double ) > _cb;
    boost::thread _thread;
    void FUNKMASTER_thread_func();
    bool needsToSeppuku;

    xn::Context g_Context;
    xn::ScriptNode g_scriptNode;
    xn::DepthGenerator g_DepthGenerator;
    xn::UserGenerator g_UserGenerator;
    XnBool g_bNeedPose;
    XnChar g_strPose[20];

    //callback VOODOO
    void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/);void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/);void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/);void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/);void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/);
  public:
    void setCallback(
        boost::function< void( int, double, double, double ) > headCB );
    int Start();
    void Shutdown();
    Lurn2SpielNub();
    ~Lurn2SpielNub();static void XN_CALLBACK_TYPE new_user(xn::UserGenerator&, XnUserID, void*);static void XN_CALLBACK_TYPE lost_user(xn::UserGenerator&, XnUserID, void*);static void XN_CALLBACK_TYPE pose(xn::PoseDetectionCapability&, const XnChar*, XnUserID, void*);static void XN_CALLBACK_TYPE cal_start(xn::SkeletonCapability&, XnUserID, void*);static void XN_CALLBACK_TYPE cal_complete(xn::SkeletonCapability&, XnUserID, XnCalibrationStatus, void*);
  };
}

void printhead( int, double, double, double );
void noop( int, double, double, double );
void noopint( int );

#endif
