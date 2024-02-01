#!/bin/bash
cd cppkafka
rmdir -rf build || true
mkdir -p build; cd build
cmake ..; make
cp src/lib/libcppkafka.so.* ../../lib
cd ../../lib
ln -s libcppkafka.so.* libcppkafka.so
cd ..
#rm -rf cppkafka

