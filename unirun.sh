#!/bin/bash

STARTING_DIR=$(pwd)
cd Build
make &&
    cd Glitter &&
    ./Glitter
cd $STARTING_DIR
