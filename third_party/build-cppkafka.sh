#!/bin/bash
set -- /usr/lib/libcppkafka.so*
if [ -f "$1" ]; then
    echo "/usr/lib/libcppkafka.so* exists!!!"
    echo "remove it first!!!"
    exit 1
fi
cd cppkafka
rm -rf build || true
mkdir -p build; cd build
cmake .. -DCPPKAFKA_DISABLE_EXAMPLES=ON ; make
if (( $? != 0)); then
    echo "!!! ERROR !!!"
    exit $?
fi
sudo cp lib/libcppkafka.so.* /usr/lib
sudo ln -s /usr/lib/libcppkafka.so.* /usr/lib/libcppkafka.so
cd ../..
#rm -rf cppkafka

