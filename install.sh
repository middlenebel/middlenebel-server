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
git --version >/dev/null 2>&1
if (( $? != 0)); then
    echo "!!! git not found !!!"
    doExit=1
fi
apt --version >/dev/null 2>&1
if (( $? != 0)); then
    echo "!!! apt not found !!!"
    doExit=1
fi
docker --version >/dev/null 2>&1
if (( $? != 0)); then
    echo "!!! docker not found !!!"
    echo "See: https://docs.docker.com/desktop/wsl/"
    doExit=1
fi
echo
kubectl version >/dev/null 2>&1 
if (( $? != 0)); then
    echo "!!! kubectl not found !!!"
    echo "See: https://kubernetes.io/docs/tasks/tools/install-kubectl-linux/"
    doExit=1
fi
echo
if (( $doExit != 0)); then
    echo "Required apt/docker/kubectl not found... Aborting installation!"
    exit 1
else
    echo "Ok!!! docker & kubectl detected!"
fi

read -p "Do you like continue with the installation [y/N]? " yesNoOption

if [ "$yesNoOption" = "y" ]; then
    echo "-------------------------- Update apt"
    sudo apt update

    echo "-------------------------- Install g++"
    sudo apt install g++

    echo "-------------------------- Install Make"
    sudo apt install make

    echo "-------------------------- Install CMake"
    sudo apt install cmake

    echo "-------------------------- Install Boost"
    sudo apt install libboost-all-dev

    echo "-------------------------- Install JSon"
    sudo apt-get install libjsoncpp-dev

    echo "-------------------------- Install LibRdKafka"
    sudo apt-get install -y librdkafka-dev

    echo "-------------------------- Install MySQL Connector"
    sudo apt-get install libmysqlcppconn-dev

    echo "-------------------------- Download, build & Install CppKafka"
    sudo apt install libssl-dev
    #cd ..
    #cd ..
    git clone https://github.com/mfontanini/cppkafka.git
    cd cppkafka
    mkdir build
    cd build
    cmake ..
    make
    sudo cp src/lib/libcppkafka.so.* /usr/lib
    sudo ln -s /usr/lib/libcppkafka.so.0.4.1 /usr/lib/libcppkafka.so
    cd .. #cd ../..
    cd .. #cd ../..

    echo "-------------------------- Build Nebel Web Server"
    docker build -t nebel-web-server .
    echo "-------------------------- Build Nebel Web Server"
    docker build -t nebel-web-server .

    echo "-------------------------- Build Middlenebel"
    cd middlenebel-server    
    make all

    echo "Installation finished!"
    echo "You can start the server with ./main"
fi
