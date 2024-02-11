#!/usr/bin/make -f

CFLAGS2=-Wall -g -Wno-comment -Wsign-compare -fPIC -std=c++17 -lstdc++fs 

HTTPFLAGS=-I/usr/include/c++/12/bits
HTTPSERVER=inc/wrappers/HttpServer.hpp 
HTTPCLIENT=inc/wrappers/HttpClient.hpp

CFLAGS=-Wall -g -Wno-comment -Wsign-compare -fPIC -std=c++17 -lstdc++fs ${HTTPFLAGS}

PLUGINS=-shared Util.o Lexical.o Component.o Core.o

all: nebel proxy

Component.o: Component.cpp inc/Component.hpp inc/Core.hpp inc/Control.hpp inc/utils/Util.hpp ${HTTPSERVER}
	g++ -c Component.cpp ${CFLAGS2} ${HTTPFLAGS}
Control.o: Control.cpp inc/Control.hpp
	g++ -c Control.cpp ${CFLAGS2} ${HTTPFLAGS}
Lexical.o: Lexical.cpp inc/Lexical.hpp inc/Core.hpp inc/utils/Util.hpp
	g++ -c Lexical.cpp ${CFLAGS2} ${HTTPFLAGS}
Config.o: Config.cpp ./inc/Core.hpp ./inc/Component.hpp ./inc/utils/Util.hpp ./inc/Config.hpp
	g++ -c Config.cpp ${CFLAGS2} ${HTTPFLAGS}
Util.o: Util.cpp inc/utils/Util.hpp inc/Core.hpp
	g++ -c Util.cpp ${CFLAGS2} ${HTTPFLAGS}
Core.o: Core.cpp inc/utils/Util.hpp inc/Config.hpp inc/PortForward.hpp inc/Core.hpp ${HTTPSERVER}
	g++ -c Core.cpp ${CFLAGS2} ${HTTPFLAGS}

nebel: NebelDocker.cpp Component.o Control.o Lexical.o Config.o Util.o Core.o ${HTTPSERVER}
	g++ -o nebel NebelDocker.cpp *.o ${CFLAGS2} ${HTTPFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -std=c++17 -Wall -Wextra -pthread -DCPPHTTPLIB_OPENSSL_SUPPORT -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lssl -lcrypto -DCPPHTTPLIB_ZLIB_SUPPORT -lz -DCPPHTTPLIB_BROTLI_SUPPORT -I/usr/local/opt/brotli/include -L/usr/local/opt/brotli/lib -lbrotlicommon -lbrotlienc -lbrotlidec
	
proxy: ProxyNebel.cpp Component.o Control.o Lexical.o Config.o Util.o Core.o \
		${HTTPSERVER} ${HTTPCLIENT}
	g++ -o proxy ProxyNebel.cpp *.o ${CFLAGS2} ${HTTPFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -std=c++17 -Wall -Wextra -pthread -DCPPHTTPLIB_OPENSSL_SUPPORT -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lssl -lcrypto -DCPPHTTPLIB_ZLIB_SUPPORT -lz -DCPPHTTPLIB_BROTLI_SUPPORT -I/usr/local/opt/brotli/include -L/usr/local/opt/brotli/lib -lbrotlicommon -lbrotlienc -lbrotlidec

all-old: build

build: buildApp buildPlugins  

buildApp:
	g++ -c ${CFLAGS} *.cpp 
	g++ -o main *.o ${CFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so 

buildPlugins:
	g++ -o plugins/Platform.so ${CFLAGS} plugins/Platform/DockerDesktop.cpp plugins/Platform/K8S.cpp plugins/Platform/Platform.cpp plugins/Platform/K8SDeployment.cpp plugins/Platform/K8SNamespace.cpp plugins/Platform/K8SService.cpp plugins/Platform/K8SLabel.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so
	g++ -o plugins/K8S-Kafka.so ${CFLAGS} plugins/Kafka/Kafka.cpp plugins/Kafka/KafkaProducer.cpp plugins/Kafka/KafkaConsumer.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so /usr/lib/x86_64-linux-gnu/librdkafka.so ../third_party/cppkafka/build/src/lib/libcppkafka.so 
	g++ -o plugins/NebelComp.so ${CFLAGS} plugins/NebelComp/NebelComp.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so
	g++ -o plugins/shared.so ${CFLAGS} plugins/shared.cpp $(PLUGINS) /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so
	g++ -o plugins/K8S-MySQL.so ${CFLAGS} plugins/MySQL/MySQL.cpp $(PLUGINS) /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so

run:
	./main

clean:
	rm outyamls/* *.log *.out *.o plugins/*.so main proxy nebel 2> /dev/null || true
