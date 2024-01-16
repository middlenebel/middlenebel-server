#!/usr/bin/make -f

CFLAGS=-Wall -g -Wcomment
#ICFLAG=-Iinc"

all: build

build: buildPlugins buildApp 

buildApp:
	g++ *.cpp ${CFLAGS} -o main

buildPlugins:
	g++ Util.cpp Lexical.cpp Core.cpp Control.cpp Component.cpp DockerDesktop.cpp plugins/Platform.cpp -Wall -g -fPIC -shared -o plugins/Platform.so -Wcomment
run:
	./main

clean:
	rm *.o 2> /dev/null || true
	rm plugins/*.so 2> /dev/null || true
	rm main 2> /dev/null || true
