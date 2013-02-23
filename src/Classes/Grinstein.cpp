#include <stdexcept>

#include "Grinstein.hpp"
#include "Cameras.hpp"
#include "Scene.hpp"
#include "Screen.hpp"

// Class-wide state variables.
Scene    *Grinstein::rootscene    = NULL;
Screen   *Grinstein::myscreen     = NULL;
Cameras  *Grinstein::camlist      = NULL;
Settings *Grinstein::eng_settings = NULL;

/**
   Default constructor. Cannot be called, this is a singleton class.
**/
Grinstein::Grinstein( void ) {
  throw std::logic_error( "Cannot construct a Grinstein object. There can only be one." );
}

/**
   Assignment operator. Cannot be used. This is a singleton class.
   @param assign Not used.
   @return Will always throw an exception.
**/
Grinstein &Grinstein::operator=( Grinstein &copy ) {
  throw std::logic_error( "Cannot assign a Grinstein to another; You can seriously only have one!" );
}

/** 
    Copy constructor. Cannot be called.
    @param copy Not used.
    @return Will always throw an exception.
**/
Grinstein::Grinstein( Grinstein &copy ) {
  throw std::logic_error( "Grinstein is a Singleton. You can't make more!" );
}

/**
   Retrieves a pointer to the Camlist.
   Creates it if it does not exist.

   @return A pointer to the CamList.
**/
Cameras *Grinstein::GetCameras( void ) {

  if (!camlist)
    camlist = &GetScreen()->camList;

  return camlist;

}

/**
   Retrieves a pointer to the Root Scene.
   Creates it if it does not exist.

   @return A pointer to the root-level scene.
**/
Scene *Grinstein::GetScene( void ) {

  //static Scene *rootscene = NULL;
  
  if (!rootscene)
    rootscene = new Scene;
  
  return rootscene;

}

/**
   Retrieves a pointer to the root Screen object.
   Creates it if it does not exist.

   @return A pointer to the root-level screen.
**/
Screen *Grinstein::GetScreen( void ) {

  //static Scene *rootscene = NULL;
  
  if (!myscreen)
    myscreen = new Screen( 0, 0 );
  
  return myscreen;

}

Settings *Grinstein::GetSettings( void ) {

  if (!eng_settings)
    eng_settings = new Settings;
  
  return eng_settings;

}
