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
echo " g++, Make, libboost-all-dev, librdkafka-dev"
echo
echo "You will need administrative rights to install in your system!"
echo

read -p "Do you like continue with the installation [y/N]? " yesNoOption

if [ $yesNoOption -eq "y" ] 
then
    exit 0
fi

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
cd ..
git clone https://github.com/mfontanini/cppkafka.git
cd cppkafka
mkdir build
cd build
cmake ..
make
sudo cp src/lib/libcppkafka.so /usr/lib
cd ../..
cd middlenebel-server


