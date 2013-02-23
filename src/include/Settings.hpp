#ifndef __SETTINGS_HPP
#define __SETTINGS_HPP

#include <string>
#include <map>

typedef std::map<std::string,bool> smap_t;

class Settings {

public:
  void Set( std::string Option, bool value );
  bool Get( std::string Option );
  bool Exists( std::string Option );

private:

  std::map< std::string, bool > smap;

};

#endif
