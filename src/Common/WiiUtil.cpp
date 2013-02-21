/**
   Based heavily on WiiC's usage example, whose header appears below.
   This file was written by John Huston, <jhuston@cs.uml.edu>
   01 Dec 20122. Only 11 days until the end of the world!
**/

/*
 *	This file is part of WiiC, written by:
 *		Gabriele Randelli
 *		Email: randelli@dis.uniroma1.it
 *
 *	Copyright 2010
 *
 *	This file is based on WiiuseCpp, written By:
 *		James Thomas
 *		Email: jt@missioncognition.net
 *
 *	Copyright 2009
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "WiiUtil.h"
#include "vec.hpp"
#include "globals.h" /* Some math constants and macros. */
#include <wiicpp.h>
#include <iostream>

#include <sys/time.h>


using std::cerr;
using Angel::vec3;
vec3 bb_magnitudes;
wiiPollData PollResults;

int LED_MAP[4] = {CWiimote::LED_1, CWiimote::LED_2, CWiimote::LED_3, CWiimote::LED_4};

bool initWii( CWii &wii ) {
  
  std::vector<CWiimote>* wiimotes;
  size_t found;
  
  for ( size_t attempts = 0; attempts < 5; ++attempts ) {
    
    std::cerr << "Please place at least one Wii Remote and one Wii Balance Board in discoverable mode.\n";
    std::cerr << "The Wii Remote can be placed in discoverable mode by pressing 1+2 simultaneously.\n";
    std::cerr << "The Balance Board has a SYNC button under the battery cover on the bottom.\n";
    
    found = wii.Find( 5 );
    
    if (found < 2) {
      cout << "Found " << found << " wiimote device(s), looking for 2+\n";
      continue;
    }
    
    std::vector<CWiimote>::iterator it;
    size_t index;
    wiimotes = &(wii.Connect( true ));
    
    for (index = 0, it = wiimotes->begin();
	 it != wiimotes->end();
	 ++it, ++index ) {
      CWiimote &wm = *it;
      /*CExpansionDevice &CED = wm.ExpansionDevice;*/
      wm.SetLEDs(LED_MAP[index]);;
    }
    return true; /* We're all good! */
  }
  return false; /* Couldn't find 2+ devices. */
}

/**
   Not currently used,
   But here for reference.
**/
void HandleEvent( CWiimote &wm ) {
  char prefixString[64];
  sprintf(prefixString, "Controller [%i]: ", wm.GetID());
  int exType = wm.ExpansionDevice.GetType();

  if(wm.Buttons.isJustPressed(CButtons::BUTTON_MINUS)) {
    wm.SetMotionSensingMode(CWiimote::ON);
    wm.IR.SetMode(CIR::ON);
    wm.EnableMotionPlus(CWiimote::ON);
    PollResults.Reset_Camera = true;
  }

  // if the accelerometer is turned on then print angles
  if(wm.isUsingACC()) {
    float pitch, roll, yaw;
    wm.Accelerometer.GetOrientation(pitch, roll, yaw);
    PollResults.wr_thetas.x = pitch;
    PollResults.wr_thetas.y = yaw;
    PollResults.wr_thetas.z = roll;
    printf("%s wiimote roll = %f\n", prefixString, roll);
    printf("%s wiimote pitch = %f\n", prefixString, pitch);
    printf("%s wiimote yaw = %f\n", prefixString, yaw);
  }
  
  // if the Motion Plus is turned on then print angles
  if(wm.isUsingMotionPlus()) {
    float roll_rate, pitch_rate, yaw_rate;
    wm.ExpansionDevice.MotionPlus.Gyroscope.GetRates(roll_rate,pitch_rate,yaw_rate);
    PollResults.wr_rates.x = pitch_rate;
    PollResults.wr_rates.y = yaw_rate;
    PollResults.wr_rates.z = roll_rate;
    printf("%s motion plus roll rate = %f\n", prefixString,roll_rate);
    printf("%s motion plus pitch rate = %f\n", prefixString,pitch_rate);
    printf("%s motion plus yaw rate = %f\n", prefixString,yaw_rate);
  }

  // if(IR tracking is on then print the coordinates
  if(wm.isUsingIR()) {
    std::vector<CIRDot>::iterator it;
    int x, y;
    int index;
    printf("%s Num IR Dots: %i\n", prefixString, wm.IR.GetNumDots());
    printf("%s IR State: %u\n", prefixString, wm.IR.GetState());
    std::vector<CIRDot>& dots = wm.IR.GetDots();
    for(index = 0, it = dots.begin(); it != dots.end(); ++index, ++it) {
      if((*it).isVisible()) {
	(*it).GetCoordinate(x, y);
	printf("%s IR source %i: (%i, %i)\n", prefixString, index, x, y);
	
	wm.IR.GetCursorPosition(x, y);
	printf("%s IR cursor: (%i, %i)\n", prefixString, x, y);
	printf("%s IR z distance: %f\n", prefixString, wm.IR.GetDistance());
      }
    }
  }
  
  if(exType == wm.ExpansionDevice.TYPE_NUNCHUK) {
    float pitch, roll, yaw;
    float angle, magnitude;
    
    CNunchuk &nc = wm.ExpansionDevice.Nunchuk;
    
    sprintf(prefixString, "Nunchuk [%i]: ", wm.GetID());
    
    if(nc.Buttons.isPressed(CNunchukButtons::BUTTON_C)) {
      printf("%s C pressed\n", prefixString);
    }
    
    if(nc.Buttons.isPressed(CNunchukButtons::BUTTON_Z)) {
      printf("%s Z pressed\n", prefixString);
    }
    
    nc.Accelerometer.GetOrientation(pitch, roll, yaw);
    printf("%s roll = %f\n", prefixString, roll);
    printf("%s pitch = %f\n", prefixString, pitch);
    printf("%s yaw = %f\n", prefixString, yaw);
    
    nc.Joystick.GetPosition(angle, magnitude);
    printf("%s joystick angle = %f\n", prefixString, angle);
    printf("%s joystick magnitude = %f\n", prefixString, magnitude);
  }
  if(exType == wm.ExpansionDevice.TYPE_BALANCE_BOARD) WiiHandleBB( wm );
}


void WiiHandleBB( CWiimote &wm ) {
  enum bb_sensor { 
    TOP_LEFT,
    TOP_RIGHT,
    BOT_LEFT,
    BOT_RIGHT,
    TOT_WEIGHT
  };

  static bool tare = false;
  static float tare_val[5] = { 0, 0, 0, 0, 0 };
  static float raw_val[5] = { 0, 0, 0, 0, 0 };
  static float adj_val[5] = { 0, 0, 0, 0, 0 };
  static size_t tare_polls = 0;

  if(wm.Buttons.isPressed(CButtons::BUTTON_A)) {
    std::cerr << "Enabling Tare\n";
    tare = true;
    tare_polls = 0;
    for (size_t i = 0; i < 5; ++i ) tare_val[i] = 0;
    PollResults.bb_magnitudes = vec3( 0, 0, 0 );
  }
  CBalanceBoard &bb = wm.ExpansionDevice.BalanceBoard;
  bb.WeightSensor.GetWeight( raw_val[TOT_WEIGHT],
			     raw_val[TOP_LEFT],
			     raw_val[TOP_RIGHT],
			     raw_val[BOT_LEFT],
			     raw_val[BOT_RIGHT] );

  /* Compute our adjusted values. */
  adj_val[TOT_WEIGHT] = 0;
  for ( size_t i = 0; i < 4; ++i ) {
    adj_val[i] = raw_val[i] - tare_val[i];
    adj_val[TOT_WEIGHT] += adj_val[i];
  }
  
  /* We are now either going to zero the scale, 
     or ignore the inputs if they are too small. */
  if (tare) {
    for ( size_t i = 0; i < 4; ++i ) tare_val[i] += raw_val[i];
    if (++tare_polls >= 1000) {
      std::cerr << "Tare complete.\n";
      tare_val[4] = 0;
      tare_polls = 0;
      tare = false;
      for ( size_t i = 0; i < 4; ++i ) {
	tare_val[i] = tare_val[i] / 1000;
	std::cerr << "Tare[" << i << "] == " << tare_val[i] << ".\n";
	tare_val[4] += tare_val[i];
      }
    }
    return; /* Return early: do not compute anything with weird half-tared values. */
  } else if (adj_val[TOT_WEIGHT] < 10) {
    PollResults.bb_magnitudes = vec3( 0, 0, 0 );
    return;  
  }

  /* Compute X and Y magnitudes. */
  float surge_pct = (adj_val[TOP_LEFT] + adj_val[TOP_RIGHT]
		     -adj_val[BOT_LEFT] - adj_val[BOT_RIGHT]) / adj_val[TOT_WEIGHT];
  float sway_pct = (adj_val[TOP_RIGHT] + adj_val[BOT_RIGHT]
		    -adj_val[TOP_LEFT] - adj_val[BOT_LEFT]) / adj_val[TOT_WEIGHT];

  /* Even with zeroing the scale and ignoring small weights,
     Sometimes the sensors report something weird.
     With this we ensure that we've normalized to the [-1,1] range. */
  if (surge_pct < -1) surge_pct = -1;
  else if (surge_pct > 1) surge_pct = 1;
  if (sway_pct < -1) sway_pct = -1;
  else if (sway_pct > 1) sway_pct = 1;

  PollResults.bb_magnitudes.y = sway_pct;
  PollResults.bb_magnitudes.z = surge_pct;

  if (0) {
    printf( "Balance Board Raw Weights: {" );
    for ( size_t i = 0; i < 5; ++i ) printf( "%6f, ", raw_val[i] );
    printf( "}\n" );
    printf( "Balance Board Adj Weights: {" );
    for ( size_t i = 0; i < 5; ++i ) printf( "%6f, ", adj_val[i] );
    printf( "}\n" );  
    printf( "SURGE: %6f; SWAY: %6f\n", surge_pct, sway_pct );
  }
  
}


void enableRemote( CWiimote &wm ) {

  wm.SetMotionSensingMode(CWiimote::ON);
  wm.IR.SetMode(CIR::ON);
  //wm.EnableMotionPlus(CWiimote::ON);
  
}


void CalibrateGyro( CWii &wii ) {

    std::vector<CWiimote>& wiimotes = wii.GetWiimotes();
    std::vector<CWiimote>::iterator it;

    for(it = wiimotes.begin(); it != wiimotes.end(); ++it) {
      CWiimote& wiimote = *it;
      if (wiimote.isUsingMotionPlus()) {
	wiimote.EnableMotionPlus(CWiimote::ON);
      }  
    }

}


void pollWii( CWii &wii, bool CalibrateGyro ) {

  struct timeval start, end;
  PollResults.Reset_Camera = false;

  //Poll the wiimotes to get the status like pitch or roll
  if (0) gettimeofday(&start,NULL);
  if(wii.Poll()) {

    std::vector<CWiimote>& wiimotes = wii.GetWiimotes();
    std::vector<CWiimote>::iterator it;

    for(it = wiimotes.begin(); it != wiimotes.end(); ++it) {

      // Use a reference to make working with the iterator handy.
      CWiimote& wiimote = *it;
      
      switch(wiimote.GetEvent()) {
	
      case CWiimote::EVENT_EVENT:
	if (wiimote.ExpansionDevice.GetType() ==
	    wiimote.ExpansionDevice.TYPE_BALANCE_BOARD)
	  WiiHandleBB(wiimote);
	HandleEvent( wiimote );
	break;

      case CWiimote::EVENT_CONNECT:
	std::cerr << "Device connecting.\n";
	break;
      case CWiimote::EVENT_UNEXPECTED_DISCONNECT:
	std::cerr << "Unexpected disconnect.\n";
	break;
      case CWiimote::EVENT_NUNCHUK_INSERTED:
	enableRemote(wiimote);
	std::cerr << "Nunchuk has become available.\n";
	break;
      case CWiimote::EVENT_NUNCHUK_REMOVED:
	std::cerr << "Nunchuk has been removed.\n";
	break;
      case CWiimote::EVENT_BALANCE_BOARD_INSERTED:
	std::cerr << "A balance board has become available.\n";
	wiimote.SetLEDs(LED_MAP[3]);;
	wiimote.SetLEDs(LED_MAP[3]);;
	break;
      case CWiimote::EVENT_BALANCE_BOARD_REMOVED:
	std::cerr << "A balance board has been removed.\n";
	break;

      case CWiimote::EVENT_CLASSIC_CTRL_INSERTED:
      case CWiimote::EVENT_MOTION_PLUS_INSERTED:
	enableRemote(wiimote);
	std::cerr << "A peripheral has been attached.\n";
	break;
	
      case CWiimote::EVENT_CLASSIC_CTRL_REMOVED:
      case CWiimote::EVENT_MOTION_PLUS_REMOVED:
	std::cerr << "A peripheral has been removed.\n";
	break;

      default:
	break;
      }
    }
  } /** if (wii.Poll()) **/

  /* Debug section for polltime */
  if (0) {
    gettimeofday(&end,NULL);
    size_t i, j;
    if ((i = end.tv_sec - start.tv_sec) == 0)
      j = end.tv_usec - start.tv_usec;
    else if ((j = end.tv_usec - start.tv_usec) < 0) {
      i--;
      j = 1000000 + j;
    }
    fprintf( stderr, "polltime: %lus%luu\n", i, j );
  }

}
