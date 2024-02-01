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
    echo "-------------------------- Update apt"
    apt update

    echo "-------------------------- Install g++"
    apt install g++

    echo "-------------------------- Install Make"
    apt install make

    echo "-------------------------- Install CMake"
    apt install cmake

    echo "-------------------------- Install Boost"
    apt install libboost-all-dev

    echo "-------------------------- Install JSon"
    apt-get install libjsoncpp-dev

    echo "-------------------------- Install LibRdKafka"
    apt-get install -y librdkafka-dev

    echo "-------------------------- Install MySQL Connector"
    apt-get install libmysqlcppconn-dev

    echo "-------------------------- Download, build & Install CppKafka"
    apt install libssl-dev
    git clone https://github.com/mfontanini/cppkafka.git
    cd cppkafka
    mkdir build
    cd build
    cmake ..
    make
    cp src/lib/libcppkafka.so.* /usr/lib
    ln -s /usr/lib/libcppkafka.so.0.4.1 /usr/lib/libcppkafka.so
    cd ../..

    echo "-------------------------- Build Nebel Web Server"
    docker build -t nebel-web-server .

    echo "-------------------------- Build Middlenebel"
    make all

    echo "Installation finished!"
    echo "You can start the server with ./main"
fi
