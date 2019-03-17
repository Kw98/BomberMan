#!/bin/sh

mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make && cd -

# mkdir build 

# cd build

# cmake ..

# make

# cd ..

# rm -rf build/