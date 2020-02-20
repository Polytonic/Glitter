#!/bin/bash

clang-format -i src/*pp &&
    clang-format -i src/*pp &&
    clang-format -i src/learnopengl/*h &&
    clang-format -i src/boids/*pp &&
    clang-format -i src/tracer/*pp
