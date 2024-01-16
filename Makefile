#!/usr/bin/make -f

CFLAGS=-Wall -g -Wno-comment -Wsign-compare -fPIC

PLUGINS=-shared Util.o Lexical.o Component.o Core.o

all: build

build: buildApp buildPlugins  

buildApp:
	g++ -c -fPIC *.cpp ${CFLAGS}
	g++ -o main *.o ${CFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so 

buildCommon:
	g++ -c -fPIC Util.cpp Lexical.cpp Component.cpp Core.cpp ${CFLAGS}

buildMain:
	g++ -c -fPIC main.cpp ${CFLAGS}
	g++ -o main *.o ${CFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so 

buildPlugins:
	g++ -o plugins/Platform.so ${CFLAGS} plugins/Platform/DockerDesktop.cpp plugins/Platform/K8S.cpp plugins/Platform/Platform.cpp plugins/Platform/K8SDeployment.cpp plugins/Platform/K8SNamespace.cpp plugins/Platform/K8SService.cpp plugins/Platform/K8SLabel.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so
	g++ -o plugins/K8S-Kafka.so ${CFLAGS} plugins/Kafka/Kafka.cpp plugins/Kafka/KafkaProducer.cpp plugins/Kafka/KafkaConsumer.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so ./lib/libcppkafka.so.0.4.0 /usr/lib/x86_64-linux-gnu/librdkafka.so
	g++ -o plugins/NebelComp.so ${CFLAGS} plugins/NebelComp/NebelComp.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so
	g++ -o plugins/shared.so ${CFLAGS} plugins/shared.cpp $(PLUGINS) /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so
	g++ -o plugins/K8S-MySQL.so ${CFLAGS} plugins/MySQL/MySQL.cpp $(PLUGINS) /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so

buildMySQL:
	g++ -o plugins/K8S-MySQL.so ${CFLAGS} plugins/MySQL/MySQL.cpp $(PLUGINS) /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so

buildShared:
	g++ -o plugins/shared.so ${CFLAGS} plugins/shared.cpp $(PLUGINS) /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so

buildPlatform:
	g++ -o plugins/Platform.so ${CFLAGS} plugins/Platform/DockerDesktop.cpp plugins/Platform/K8S.cpp plugins/Platform/Platform.cpp plugins/Platform/K8SDeployment.cpp plugins/Platform/K8SNamespace.cpp plugins/Platform/K8SService.cpp plugins/Platform/K8SLabel.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so 
	
buildKafka:
	g++ -o plugins/K8S-Kafka.so ${CFLAGS} plugins/Kafka/Kafka.cpp plugins/Kafka/KafkaProducer.cpp plugins/Kafka/KafkaConsumer.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so ./lib/libcppkafka.so.0.4.0 /usr/lib/x86_64-linux-gnu/librdkafka.so

buildNebelComp:
	g++ -o plugins/NebelComp.so ${CFLAGS} plugins/NebelComp/NebelComp.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so 

run:
	./main

clean:
	rm *.o 2> /dev/null || true
	rm plugins/*.so 2> /dev/null || true
	rm main 2> /dev/null || true
