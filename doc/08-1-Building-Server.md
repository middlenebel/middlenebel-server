
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
The middlenebel-server includes a static version of the angular front application.

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

docker run -p 8080:8080 nebel-docker & 

docker run -dit nebel-docker
docker ps
docker attach xxxxxx

docker context use desktop-linux

Answer for ubuntu users Please create docker context

docker context create desktop-linux --description "Docker Desktop" --docker "host=unix:///home/YOUR_USER_NAME/.docker/desktop/docker.sock"

cd  C:\Program Files\Docker\Docker
DockerCli.exe -SwitchDaemon

# Third_party 
This application include several third_party modules.

To update the third_party modules you can use:

git submodule update --init --recursive

But this not give warranties because third_parties can be more avanced in changes not supported for this version.

**boost**
https://stackoverflow.com/questions/3016448/how-can-i-get-cmake-to-find-my-alternative-boost-installation
I was finally able to get what I wanted with

cmake -DCMAKE_INSTALL_PREFIX=$TARGET \
    -DBoost_NO_BOOST_CMAKE=TRUE \
    -DBoost_NO_SYSTEM_PATHS=TRUE \
    -DBOOST_ROOT:PATHNAME=$TARGET \
    -DBoost_LIBRARY_DIRS:FILEPATH=${TARGET}/lib

For cppkafka

cmake .. -DBoost_NO_BOOST_CMAKE=TRUE \
    -DBoost_NO_SYSTEM_PATHS=TRUE \
    -DBOOST_ROOT:PATHNAME=$TARGET \
    -DCPPKAFKA_DISABLE_EXAMPLES=ON

cpp-httplib
    mkdir -p build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	sudo cmake --build . --target install

    punksystec@LAPTOP-CPH12905:~/projects/cpp-httplib/build$ sudo cmake --build . --target install
[sudo] password for punksystec:
Install the project...
-- Install configuration: ""
-- Installing: /usr/local/include/httplib.h
-- Installing: /usr/local/lib/cmake/httplib/httplibConfig.cmake
-- Installing: /usr/local/lib/cmake/httplib/httplibConfigVersion.cmake
-- Installing: /usr/local/lib/cmake/httplib/FindBrotli.cmake
-- Installing: /usr/local/lib/cmake/httplib/httplibTargets.cmake
punksystec@LAPTOP-CPH12905:~/projects/cpp-httplib/build$