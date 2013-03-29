#ifndef __WII_H
#define __WII_H

#include <wiicpp.h>
#include "vec.hpp"

typedef struct {
  Angel::vec3 bbMagnitudes;
  Angel::vec3 wrThetas;
  Angel::vec3 wrRates;
  bool resetCamera;
} WiiPollData;

extern int ledMap[4];
extern Angel::vec3 bbMagnitudes;
extern WiiPollData pollResults;

bool initWii( CWii &wii );
void pollWii( CWii &wii, bool CalibrateGyro = false );
void wiiHandleBB( CWiimote &wm );
void calibrateGyro( CWii &wii );

#endif
