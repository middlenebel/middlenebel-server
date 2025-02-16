#!/usr/bin/make -f

CFLAGS2=-Wall -g -Wno-comment -Wsign-compare -fPIC -std=c++17 -lstdc++fs 

HTTPFLAGS=-I/usr/include/c++/12/bits #-lbrotlienc -lbrotlidec
HTTPSERVER=inc/wrappers/HttpServer.hpp 
HTTPCLIENT=inc/wrappers/HttpClient.hpp

CFLAGS=-Wall -g -Wno-comment -Wsign-compare -fPIC -std=c++17 -lstdc++fs ${HTTPFLAGS}

PLUGINSOBG=Util.o Lexical.o Component.o Core.o
PLUGINSFLG=-shared

PLUGINS=${PLUGINSFLG} ${PLUGINSOBG}

SHAREPLUGINS= -shared Util.so Lexical.so Component.so Core.so

all: commons nebel proxy buildPlugins

commons:
	g++ -c Component.cpp ${CFLAGS2} ${HTTPFLAGS}
	g++ -c ComponentNoCore.cpp ${CFLAGS2} ${HTTPFLAGS}
	g++ -c Control.cpp ${CFLAGS2} ${HTTPFLAGS}
	g++ -c Lexical.cpp ${CFLAGS2} ${HTTPFLAGS}
	g++ -c Config.cpp ${CFLAGS2} ${HTTPFLAGS}
	g++ -c Util.cpp ${CFLAGS2} ${HTTPFLAGS}
	g++ -c Core.cpp ${CFLAGS2} ${HTTPFLAGS}
	g++ -c Browser.cpp ${CFLAGS2} ${HTTPFLAGS}

nebel: NebelDocker.cpp Component.o ComponentNoCore.o Control.o Lexical.o Config.o Util.o Core.o ${HTTPSERVER}
#	g++ -o nebel NebelDocker.cpp *.o ${CFLAGS2} ${HTTPFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -std=c++17 -Wall -Wextra -pthread -DCPPHTTPLIB_OPENSSL_SUPPORT -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lssl -lcrypto -DCPPHTTPLIB_ZLIB_SUPPORT -lz -DCPPHTTPLIB_BROTLI_SUPPORT -I/usr/local/opt/brotli/include -L/usr/local/opt/brotli/lib -lbrotlicommon -lbrotlienc -lbrotlidec
#	g++ -o nebel NebelDocker.cpp *.o ${CFLAGS2} ${HTTPFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -std=c++17 -Wall -Wextra -pthread -DCPPHTTPLIB_OPENSSL_SUPPORT -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lssl -lcrypto 
	g++ -o nebel NebelDocker.cpp *.o ${CFLAGS2} ${HTTPFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -std=c++17 -Wall -Wextra -pthread 
	cp nebel ..

proxy: ProxyNebel.cpp Component.o ComponentNoCore.o Control.o Lexical.o Config.o Util.o Browser.o \
		${HTTPSERVER} ${HTTPCLIENT}
#	g++ -o proxy ProxyNebel.cpp *.o ${CFLAGS2} ${HTTPFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -std=c++17 -Wall -Wextra -pthread -DCPPHTTPLIB_OPENSSL_SUPPORT -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lssl -lcrypto -DCPPHTTPLIB_ZLIB_SUPPORT -lz -DCPPHTTPLIB_BROTLI_SUPPORT -I/usr/local/opt/brotli/include -L/usr/local/opt/brotli/lib -lbrotlicommon -lbrotlienc -lbrotlidec
#	g++ -o proxy ProxyNebel.cpp *.o ${CFLAGS2} ${HTTPFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -std=c++17 -Wall -Wextra -pthread -DCPPHTTPLIB_OPENSSL_SUPPORT -I/usr/local/opt/openssl@3/include -L/usr/local/opt/openssl@3/lib -lssl -lcrypto 
	g++ -o proxy ProxyNebel.cpp *.o ${CFLAGS2} ${HTTPFLAGS} /lib/x86_64-linux-gnu/libjsoncpp.so -std=c++17 -Wall -Wextra -pthread
	cp proxy ..
	
buildPlugins:
	g++ -o plugins/Platform.so ${CFLAGS} plugins/Platform/DockerDesktop.cpp plugins/Platform/Platform.cpp  $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so
	g++ -o plugins/K8S.so ${CFLAGS} plugins/K8S/K8S.cpp plugins/K8S/K8SDeployment.cpp plugins/K8S/K8SNamespace.cpp plugins/K8S/K8SService.cpp plugins/K8S/K8SLabel.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so
	g++ -o plugins/K8S-Kafka.so ${CFLAGS} plugins/Kafka/Kafka.cpp plugins/Kafka/KafkaProducer.cpp plugins/Kafka/KafkaConsumer.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so /usr/lib/x86_64-linux-gnu/librdkafka.so /usr/lib/libcppkafka.so 
	g++ -o plugins/NebelComp.so ${CFLAGS} plugins/NebelComp/NebelComp.cpp $(PLUGINS) /lib/x86_64-linux-gnu/libjsoncpp.so
	g++ -o plugins/shared.so ${CFLAGS} plugins/shared.cpp $(PLUGINS) /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so
	g++ -o plugins/K8S-MySQL.so ${CFLAGS} plugins/MySQL/MySQL.cpp $(PLUGINS) /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so
	cp plugins/*.so ../plugins

# ------------------------------------------------------------------------------
copy:
	cp nebel ..
	cp proxy ..
	cp plugins/*.so ../plugins

clean:
	rm outyamls/* *.log *.out *.o *.so plugins/*.so main proxy nebel 2> /dev/null || true

tests: testNebel testProxy

testProxy:
	newman run '../ProxyNebel.postman_collection.json'
	
testNebel:
	newman run '../NebelDocker.postman_collection.json'

