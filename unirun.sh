#!/bin/bash

STARTING_DIR=$(pwd)
cp Glitter/Shaders/* Build/Glitter/;
cd Build
make -j 8 &&
    cd Glitter &&
    time ./Glitter "$@"
cd $STARTING_DIR
