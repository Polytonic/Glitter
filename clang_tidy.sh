#!/bin/bash

clang-format -i Glitter/Headers/*pp &&
    clang-format -i Glitter/Sources/*pp &&
    clang-format -i Glitter/Headers/learnopengl/*h
