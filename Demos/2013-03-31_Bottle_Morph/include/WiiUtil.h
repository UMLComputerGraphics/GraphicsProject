#ifndef __WII_H
#define __WII_H

#include <wiicpp.h>
#include "vec.hpp"

typedef struct {
  Angel::vec3 bb_magnitudes;
  Angel::vec3 wr_thetas;
  Angel::vec3 wr_rates;
  bool Reset_Camera;
} wiiPollData;

extern int LED_MAP[4];
extern Angel::vec3 bb_magnitudes;
extern wiiPollData PollResults;

bool initWii( CWii &wii );
void pollWii( CWii &wii, bool CalibrateGyro = false );
void WiiHandleBB( CWiimote &wm );
void CalibrateGyro( CWii &wii );

#endif
