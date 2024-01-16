#!/usr/bin/make -f

# CFLAGS=-Wall -g -Wcomment -Wsign-compare -fPIC
CFLAGS=-Wall -g -Wno-comment -Wsign-compare -fPIC
#ICFLAG=-Iinc"

#  --cflags glib-2.0

all: build

build: buildApp buildPlugins  

buildCommon:
	g++ -c -fPIC Util.cpp Lexical.cpp Component.cpp Core.cpp ${CFLAGS}

buildMain:
	g++ -c -fPIC main.cpp ${CFLAGS}
	g++ *.o ${CFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -o main

buildApp:
	g++ -c -fPIC *.cpp ${CFLAGS}
	g++ *.o ${CFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -o main

buildPlatform:
	g++ Util.o Lexical.o Component.o plugins/Platform/DockerDesktop.cpp plugins/Platform/K8S.cpp plugins/Platform/Platform.cpp plugins/Platform/K8S_Deployment.cpp plugins/Platform/K8S_Namespace.cpp plugins/Platform/K8S_Service.cpp plugins/Platform/K8S_Label.cpp /lib/x86_64-linux-gnu/libjsoncpp.so -shared ${CFLAGS} -o plugins/Platform.so
	
buildKafka:
	g++ -c -fPIC Core.cpp ${CFLAGS}
	g++ Util.o Lexical.o Component.o Core.o plugins/Kafka/Kafka.cpp plugins/Kafka/KafkaProducer.cpp plugins/Kafka/KafkaConsumer.cpp /lib/x86_64-linux-gnu/libjsoncpp.so ./lib/libcppkafka.so.0.4.0 /usr/lib/x86_64-linux-gnu/librdkafka.so -shared ${CFLAGS} -o plugins/K8S-Kafka.so

buildPlugins:
	g++ Util.o Lexical.o Component.o plugins/Platform/DockerDesktop.cpp plugins/Platform/K8S.cpp plugins/Platform/Platform.cpp plugins/Platform/K8S_Deployment.cpp plugins/Platform/K8S_Namespace.cpp plugins/Platform/K8S_Service.cpp plugins/Platform/K8S_Label.cpp /lib/x86_64-linux-gnu/libjsoncpp.so -shared ${CFLAGS} -o plugins/Platform.so
	g++ Util.o Lexical.o Component.o plugins/Kafka/Kafka.cpp plugins/Kafka/KafkaProducer.cpp plugins/Kafka/KafkaConsumer.cpp /lib/x86_64-linux-gnu/libjsoncpp.so ./lib/libcppkafka.so.0.4.0 /usr/lib/x86_64-linux-gnu/librdkafka.so -shared ${CFLAGS} -o plugins/K8S-Kafka.so
run:
	./main

clean:
	rm *.o 2> /dev/null || true
	rm plugins/*.so 2> /dev/null || true
	rm main 2> /dev/null || true
