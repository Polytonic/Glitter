#!/bin/bash

STARTING_DIR=$(pwd)
cp Glitter/Shaders/* Build/Glitter/;
cd Build
make -j 8 &&
	     cd Glitter &&
	     time ./Glitter "$@"
RESULT=$?
cd $STARTING_DIR
exit $RESULT
