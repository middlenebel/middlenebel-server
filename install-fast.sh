#!/bin/bash

echo "------------------------"
echo "Middlenebel Installation"
echo "------------------------"
echo
echo "This script install dependencies and build Middlenebel."
echo
echo "You can abort the script and do it manually, see: doc/08-Building.md for instructions."
echo
echo "The next dependencies will be installed in your system:"
echo
echo " g++, Make, CMake, libboost, libjsoncpp, librdkafka, libmysqlcppconn, libssl, cppkafka"
echo
echo "You will need administrative rights to install in your system!"
echo
echo You need git, apt, docker and kubectl installed in you Linux/WSL2. This will NOT DO it.
echo
doExit=0
apt --version >/dev/null 2>&1
if (( $? != 0)); then
    echo "!!! apt not found !!!"
    doExit=1
fi
git --version >/dev/null 2>&1
if (( $? != 0)); then
    echo "!!! git not found !!!"
    doExit=2
fi
echo
kubectl version >/dev/null 2>&1 
if (( $? != 0)); then
    echo "!!! kubectl not found !!!"
    echo "See: https://kubernetes.io/docs/tasks/tools/install-kubectl-linux/"
    doExit=3
fi
docker --version >/dev/null 2>&1
if (( $? != 0)); then
    echo "!!! docker not found !!!"
    echo "See: https://docs.docker.com/desktop/wsl/"
    doExit=4
fi
echo
if (( $doExit == 0)); then
    echo "Ok!!! docker & kubectl detected!"
fi
if (( $doExit > 1)); then
    echo "!!! Required apt/docker/kubectl not found... Aborting installation !!!"
    exit $doExit
fi
read -p "Do you like continue with the installation [y/N]? " yesNoOption

if [ "$yesNoOption" = "y" ]; then
    echo "-------------------------- Installing dependencies..."
    apt install make cmake libboost-all-dev librdkafka-dev libssl-dev libjsoncpp-dev libmysqlcppconn-dev
    cd cppkafka
    mkdir build; cd build
    cmake ..; make
    cp src/lib/libcppkafka.so.* ../../lib
    ln -s ../../lib/libcppkafka.so.* ../../lib/libcppkafka.so
    cd ../..
    #rm -rf cppkafka
    echo "-------------------------- Building Middlenebel..."
    make all

    echo "Installation finished!"
    echo "You can start the server with ./main"
fi
