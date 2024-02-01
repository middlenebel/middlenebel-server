
# How-to Build Middlenebel-Server

git clone https://github.com/middlenebel/middlenebel-server.git

cd middlenebel-server

You can follow this guide or use the installer script: **install.sh**

## Requirements
You will need some common utilities that will not explained in this guide:

**apt, git, docker, kubectl.**

## Install g++
sudo apt install g++

## Install Make & CMake
sudo apt install make
sudo apt install cmake

## Install Boost
sudo apt install libboost-all-dev

## Install LibJson
sudo apt-get install libjsoncpp-dev

## Install LibRdKafka
sudo apt-get install -y librdkafka-dev

## Install MySQL Connector
sudo apt-get install libmysqlcppconn-dev

## Download & Build LibCppKafka
You can do this in the middlenebel-server folder or outside of it.

```
sudo apt install libssl-dev
sudo apt-get install -y libz-dev
git clone https://github.com/mfontanini/cppkafka.git
cd cppkafka
mkdir build
cd build
cmake ..
make

[...] If the make finish ok you can install the libraries:

sudo cp src/lib/libcppkafka.so.* /usr/lib
sudo ln -s /usr/lib/libcppkafka.so.* /usr/lib/libcppkafka.so
cd ../..
```

## Download & Build Middlenebel Server

(If you didn't do this before)

```
git clone https://github.com/middlenebel/middlenebel-server.git
cd middlenebel-server
```

In middlenebel-server folder:

**make all**

Installation finished!
You can start the server with ./main

## Running Front
The middlenebel-server includes a static version of the angular fornt application.

Yo can download and run the front separatelly, see **08-2-Building-Front.md** for instructions.

Or you can start the web server incuded in **'dist'** folder:

you can do it in two ways:

**1 - Using an http server (like lite-server)**
lite-server -c Config.lite-server.json 

You can change the port used in that file.

**2 - Installing the Front-Web in Docker**
For that:

Build image and install in docker:

docker build -t nebel-web-server .

Then run the server:

docker run -p 8081:80 nebel-web-server & 
