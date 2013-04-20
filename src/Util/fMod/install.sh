#!/usr/bin/env bash

if [[ "$EUID" -ne "0" ]]; then
  echo "Hey, you have to run me as root, ok?"
  exit 1
fi

arch=$(getconf LONG_BIT);
if (( "$arch" == "64" )); then
  file=libfmodex64.so;
else
  file=libfmodex.so;
fi

instdir="/usr/local/lib/fMod/";
mkdir -p "$instdir";

set -x
install $file $instdir;
ln -s "$instdir/$file" "/usr/local/lib/libfmodex.so"
ldconfig
set +x

echo "Done. Seeya!"