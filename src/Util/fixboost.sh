#!/bin/bash
set -e
set -x
[ -e boost ] && exit 0
sudo port clean boost
svn co -r 93341 http://svn.macports.org/repository/macports/trunk/dports/devel/boost
cd boost
sudo port install
sudo port activate boost @1.49.0_0
set +e
set +x0
