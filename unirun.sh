#!/bin/bash

STARTING_DIR=$(pwd)
cd Build
make -j 8 &&
    cd Glitter &&
    time ./Glitter
cd $STARTING_DIR
