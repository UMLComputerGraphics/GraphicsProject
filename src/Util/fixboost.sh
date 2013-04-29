#!/bin/bash
set -e
set -x
[ -e boost2 ] && exit 0
rm -Rf boost
sudo port clean boost
sudo sed -i '' 's/#macportsuser.*/macportsuser	root/g' /opt/local/etc/macports/macports.conf
svn co -r 93341 http://svn.macports.org/repository/macports/trunk/dports/devel/boost boost2
cd boost2
sudo port install
sudo port activate boost @1.49.0_0
set +e
set +x
