#!/bin/bash

if [ ! -d "build" ];then
mkdir build
fi

cd build
# cmake ..
cmake -DTEST=ON -DCMAKE_BUILD_TYPE=Debug ..
make

# Outputs
cp libbbmm.so ..
#cp libbbmm.so ../python-src/BeMAge
cp run-test-BbMm ..
