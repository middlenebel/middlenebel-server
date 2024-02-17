#!/bin/bash
echo "Installing cpp-httplib"
cd cpp-httplib
rm -rf build
mkdir build 
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo cmake --build . --target install
cd ../..