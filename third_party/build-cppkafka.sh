#!/bin/bash
cp /usr/lib/libcppkafka.so.* bak
rm -rf ./cppkafka/include/cppkafka/boost || true
ln -s $PWD/boost_1_82_0/boost ./cppkafka/include/cppkafka/boost || true
cd cppkafka
rm -rf build || true
mkdir -p build; cd build
cmake .. -DCPPKAFKA_DISABLE_EXAMPLES=ON ; make
if (( $? != 0)); then
    echo "!!! ERROR !!!"
    exit $?
fi
sudo rm /usr/lib/libcppkafka.so
sudo cp src/lib/libcppkafka.so.* /usr/lib
sudo ln -s /usr/lib/libcppkafka.so.* /usr/lib/libcppkafka.so
cd ../..
#rm -rf cppkafka

