#!/usr/bin/env bash

arch=$(getconf LONG_BIT);
libdir="lib"
if (( "$arch" == "64" )); then
  libdir="lib64"
  file=libfmodex64.so;
else
  file=libfmodex.so;
fi

instdir="/usr/local/$libdir";

[ -d $instdir/fMod ] && exit 0

if [[ "$EUID" -ne "0" ]]; then
  sudo $0
  exit $?
fi

mkdir -p "$instdir/fMod";

set -x
install `dirname $0`/$file $instdir/fMod;
ln -s "$instdir/fMod/$file" "$instdir/libfmodex.so"
echo "$instdir" > /etc/ld.so.conf.d/fmod.conf
ldconfig
set +x



echo "Done. Seeya!"
