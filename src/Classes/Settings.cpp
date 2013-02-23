#include "Settings.hpp"
#include <string>
#include <map>

void Settings::Set( std::string Option, bool value ) {
  smap[Option] = value;
}

bool Settings::Get( std::string Option ) {
  return smap[Option];
}

bool Settings::Exists( std::string Option ) {
  
  smap_t::iterator it;
  it = smap.find(Option);

  if (it == smap.end()) return false;
  return true;
}

