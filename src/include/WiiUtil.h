/**
 * @file WiiUtil.h
 * @author John Huston
 * @date 2013-03-29
 * @brief Wii Support Utility Header
 * @details Functions and global declarations for Wii Remote Support.
 */

#ifndef __WII_H
#define __WII_H

#include <wiicpp.h>
#include "vec.hpp"

/**
 * The WiiPollData structure contains
 * data as polled from a set of wii remote devices,
 * including balance board magnitudes, Wii Remote angles,
 * Wii Remote accelerometer rates, and a resetCamera boolean.
 */
typedef struct {
  /** Balance Board magnitude measurement in the form of a normalized vec3. **/
  Angel::vec3 bbMagnitudes;
  /** Wii Remote Angles as a triple, utilizing a vec3. **/
  Angel::vec3 wrThetas;
  /** Wii Remote accelerometer rates as a triple, utilizing a vec3. **/
  Angel::vec3 wrRates;
  /**
   * A boolean indicating whether or not the user has requested
   * a Gyroscope calibration.
   */
  bool resetCamera;
} WiiPollData;

/**
 * A list of constants that may be used to address
 * the led lights on each Wii Remote.
 */
extern int ledMap[4];

/**
 * Global poll results from the Balance Board.
 */
extern Angel::vec3 bbMagnitudes;

/**
 * A globally provided structure to capture Wii Remote information.
 */
extern WiiPollData pollResults;

/**
 * Initialize a Wii structure.
 * @param wii The Wii structure to initialize
 * @return A boolean indicating success or failure.
 */
bool initWii( CWii &wii );

/**
 * Poll a set of Wii Remotes for measurements.
 * @param wii The set of Wii remotes to poll.
 * @param CalibrateGyro Optionally, Calibrate the Gyroscope.
 */
void pollWii( CWii &wii, bool CalibrateGyro = false );

/**
 * Function to interpret the data returned from a Balance Board.
 * @param wm The handle to the WiiMote determined to be a Balance Board.
 */
void wiiHandleBB( CWiimote &wm );

/**
 * Calibrate the Gyroscope. Put the Wiimote on a flat table!
 * @param wii The collection of Wii remotes to calibrate.
 */
void calibrateGyro( CWii &wii );

#endif
