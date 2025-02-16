#!/usr/bin/bash


cd src

make all
make buildPlugins
	
cd ..
cp ./src/nebel .
cp ./src/proxy .
cp ./src/plugins/*.so ./plugins
