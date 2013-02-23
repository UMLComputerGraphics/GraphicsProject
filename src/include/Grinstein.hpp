#ifndef __GRINSTEIN_HPP
#define __GRINSTEIN_HPP

#include "Cameras.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Settings.hpp"

class Grinstein {

public:

  /*
    typedef enum singletons {
    Begin,
    SCENE_H = Begin,
    SCREEN_H,
    CAMERAS_H,
    SETTINGS_H,
    End 
  } singletons_t;
  */

  static Scene    *GetScene   ( void );
  static Screen   *GetScreen  ( void );
  static Cameras  *GetCameras ( void );
  static Settings *GetSettings( void );

private:

  Grinstein( void );
  Grinstein( Grinstein &copy );
  Grinstein &operator=( Grinstein &assign );

  static Cameras *camlist;
  static Scene *rootscene;
  static Screen *myscreen;
  static Settings *eng_settings;

};

#endif
