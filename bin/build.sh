#!/bin/bash

if [ ! -d "build" ];then
mkdir build
fi

cd build
# cmake ..
cmake -DTEST=ON -DCMAKE_BUILD_TYPE=Debug ..
make

# Outputs
cp libbemage.so ..
#cp libbemage.so ../python-src/BeMAge
cp test-BeMAge ..
