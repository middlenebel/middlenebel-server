#!/bin/bash
echo "Installing cpp-httplib"
cd cpp-httplib
rm -rf build
mkdir build 
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DHTTPLIB_USE_OPENSSL_IF_AVAILABLE=off -DHTTPLIB_USE_ZLIB_IF_AVAILABLE=off -DHTTPLIB_USE_ZLIB_IF_AVAILABLE=off ..
sudo cmake --build . --target install
cd ../..