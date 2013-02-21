#!/bin/bash

set -x
set -e
mkdir build
pushd build
cmake ../src/
make