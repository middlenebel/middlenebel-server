
#include <chrono>
#include <thread>

#include <jsoncpp/json/json.h>

#include "../../inc/utils/Util.hpp"
#include "../../inc/Core.hpp"
#include "../../inc/Component.hpp"
#include "../../incPlugins/Kafka.hpp"
#include "../../incPlugins/KafkaProducer.hpp" 

using namespace std::chrono_literals;

Kafka* kafka = nullptr;

int Kafka::kafkaNum = 0;

extern "C" void* loadPlugin(Component* parent){
    cout << "Loading plugin Kafka...\n";
    CREATE_NEBEL_PLUGIN( "kafka", kafka , Kafka(parent) );
    return (void*)kafka;
}

extern "C" void parse(void* instance ){
    Kafka* kafka = (Kafka*) instance;
    kafka->parse();
}

Kafka::Kafka(Component* parent):Component(parent){
    producer = nullptr;
    consumer = nullptr;
    init();
    // parent->addComponent(this); //Do it in loadPlugin()
}

void Kafka::init(){
    attributes[ATT_CLASSNAME]="K8S-Kafka";
    attributes[ATT_NAME] = "Kafka-"+to_string(kafkaNum++);
    portForward = false;
    LOG( "Kafka started!");
}

void Kafka::parse(){
    LOG( "Parse by Kafka...");
    bool parsedName = false;
    string key="";
    string value="";
    Core* core = (Core*) getRootComponent( this );
    while ( readToken() ){
        //DEBUG LOG( "\Kafka TOKEN: " + token );

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        else if (isToken( TK_PORT_FORWARD )){ parsePortForward(); }
        // else if (isToken( TK_PORT_FORWARD )){ 
        //     string actionPF = attributes[ATT_CLASSNAME] +"-"+TK_PORT_FORWARD;
        //     this->portForward = true; 
        //     core->executors[actionPF] = this; 
        //     this->actions[ actionPF ] = actionPF;
        // }
        else if (isToken( TK_KAFKA_PRODUCER )){ 
            core->executors[TK_KAFKA_PRODUCER] = this; 
            this->actions[ TK_KAFKA_PRODUCER ] = TK_KAFKA_PRODUCER;
        } else if (isToken( TK_KAFKA_CONSUMER )){ 
            core->executors[TK_KAFKA_CONSUMER] = this; 
            this->actions[ TK_KAFKA_CONSUMER ] = TK_KAFKA_CONSUMER;
        } else if ( !parsedName && token.compare( "" ) != 0 ){ // Gets the name of kafka server
                LOG( "Kafka " + token );
                attributes[ATT_NAME] = token;
                parsedName = true;
        } else PARSE_ATT_KEY_TOKEN(attributes, key, token);
    }
}

void Kafka::doPlay(std::list<string> *cmds){
    string nameSpace=getAtt( ATT_NAMESPACE, "nebel");
    string fileName, command, resultCommand;
    
    command = "kubectl create namespace "+nameSpace;
    cmds->push_back( systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Create namespace "+nameSpace )+"\n" );

    fileName="./cfgPlugins/deploy-zookeeper.tmp.yaml";
    command = "kubectl apply -f "+fileName+" -n "+nameSpace;
    cmds->push_back( systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Deploy zookeeper")+"\n" );
        //,fileName, Util::loadFile(fileName) );

    fileName="./cfgPlugins/deploy-kafka-broker.tmp.yaml";
    command = "kubectl apply -f "+fileName+" -n "+nameSpace;
    cmds->push_back( systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Deploy kafka-broker")+"\n" );
        //,fileName, Util::loadFile(fileName) );

    fileName="./cfgPlugins/service-zookeeper.tmp.yaml";
    command = "kubectl apply -f "+fileName+" -n "+nameSpace;
    cmds->push_back( systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Service zookeeper")+"\n" );
        //,fileName, Util::loadFile(fileName) );

    fileName="./cfgPlugins/service-kafka-broker.tmp.yaml";
    command = "kubectl apply -f "+fileName+" -n "+nameSpace;
    cmds->push_back( systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Service kafka-broker")+"\n" );
        //,fileName, Util::loadFile(fileName) );

    LOG( "K8S-Kafka "+attributes[ATT_NAME] + " deployed!" );
}
void Kafka::doDestroy(std::list<string> *cmds){
     try{
        string name=attributes[ATT_NAME];
        string nameSpace=getAtt(ATT_NAMESPACE, "default");

        string command;

        try{
            command = "kubectl delete deployment zookeeper-object -n "+nameSpace;
            cmds->push_back( systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Remove deployment zookeeper") );
        }catch(const std::exception& ex){
            LOG("Exception deleting deployment zookeeper: "+std::string(ex.what()));
        }

        try{
            command = "kubectl delete deployment kafka-broker-object -n "+nameSpace;
            cmds->push_back( systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Remove deployment kafka-broker") );
        }catch(const std::exception& ex){
            LOG("Exception deleting deployment kafka-broker: "+std::string(ex.what()));
        }

        try{
            command = "kubectl delete service zookeeper-object -n "+nameSpace;
            cmds->push_back(  systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Remove service zookeeper") );
        }catch(const std::exception& ex){
            LOG("Exception deleting service zookeeper: "+std::string(ex.what()));
        }
        try{
            command = "kubectl delete service kafka-broker-object -n "+nameSpace;
            cmds->push_back( systemCommandList( command, attributes[ATT_APP], nameSpace, "?" ,"Remove service kafka-broker") );
        }catch(const std::exception& ex){
            LOG("Exception K8S-Kafka: "+std::string(ex.what()));
        }

        LOG( "Undeploy K8S-Kafka "+attributes[ATT_NAME] ); //Can have " in the message + ": " + resultCommand);
     }catch(const std::exception& ex){
        LOG("Exception "+std::string(ex.what()));
        LOG( "ERROR Undeploy K8S-Kafka "+attributes[ATT_NAME] );
     }
}

string Kafka::execute( string json ){
    //DEBUG LOG(  "Kafka executing... ");
        
    string result = "KO";
    string message = "Command unknown";

    Json::Value actionJson;
    Json::Reader reader; 
    bool isOk = reader.parse( json, actionJson );
    if (!isOk){
        LOG( "Error parsing actions json! " + json);
        return (string)"{ \"result\" : \""+ result + "\" , \"message\" : \"" + message + "\" }";
    }

    const Json::Value actionValue = actionJson["action"];
    string actionStr = actionValue.asString();
    // DEBUG( "Kafka actionStr " + actionStr);

    if ( actionStr == TK_KAFKA_PRODUCER){
        const Json::Value dataValue = actionJson["data"];
        string dataStr = dataValue.asString();
        return executeProducer( dataStr );
    }else if ( actionStr == TK_KAFKA_CONSUMER){ //TODO
        const Json::Value dataValue = actionJson["data"];
        string dataStr = dataValue.asString();
        return executeConsumer( dataStr );
    }

    return (string)"{ \"result\" : \""+ result + "\" , \"message\" : \"" + message + "\" }";
}

string Kafka::executeProducer(string json){
    //DEBUG LOG(  "Kafka executing Producer!" );
    string result = "KO";
    string message = "Command error in Producer!";

    Json::Value dataJson;
    Json::Reader reader;
    bool isOk = reader.parse( json, dataJson );
    if (!isOk){
        LOG( "executeProducer: Error parsing data json!");
        return (string)"{ \"result\" : \""+ result + "\" , \"message\" : \"" + message + "\" }";
    }

    const Json::Value topicValue = dataJson["topic"];
    string topicStr = topicValue.asString();

    const Json::Value messageValue = dataJson["message"];
    string messageStr = messageValue.asString();

    const Json::Value modeValue = dataJson["mode"];
    string modeStr = modeValue.asString();

    //DEBUG LOG( "executeProducer: Mode " + modeStr );
    //DEBUG LOG( "executeProducer: Message " + messageStr );

    if (producer == nullptr){
        // producer = new KafkaProducer();
        CREATE_NEBEL( "kafka/producer", producer , KafkaProducer() );
    }

    if (modeStr == "Manual"){
        LOG( "executeProducer sending Manual... " + messageStr);
        producer->send( topicStr, messageStr );
        result = "OK";
        message = "Message manual sent!";
    }else if (modeStr == "Auto"){
        LOG( "executeProducer sending Auto..." );
        const Json::Value maxValue = dataJson["max"];
        int max = maxValue.asInt();

        const Json::Value intervalValue = dataJson["interval"];
        int interval = intervalValue.asInt();

        LOG("Scheduling autoSend " + to_string(max));
        producer->autoSend( topicStr, messageStr, max, interval );
        LOG("Scheduled autoSend");
        result = "OK";
        message = "Message auto scheduled!";
    }else if (modeStr == "Abort"){
        producer->abortSend = true;
    }

    return (string)"{ \"result\" : \""+ result + "\" , \"message\" : \"" + message + "\" }";
}

string Kafka::executeConsumer(string json){
    string result = "KO";
    string message = "Command error in Consumer!";

    Json::Value dataJson;
    Json::Reader reader;
    bool isOk = reader.parse( json, dataJson );
    if (!isOk){
        LOG( "executeConsumer: Error parsing data json!");
        return (string)"{ \"result\" : \""+ result + "\" , \"message\" : \"" + message + "\" }";
    }

    const Json::Value actionValue = dataJson["action"];
    string actionStr = actionValue.asString();

    //DEBUG( "executeConsumer: action " + actionStr );

    if (consumer == nullptr){
        CREATE_NEBEL( "kafka/consumer", consumer , KafkaConsumer() );
        LOG("Consumer created!");
    }

    if (actionStr == "Consume"){
        //DEBUG("Action consume");
        const Json::Value topicValue = dataJson["topic"];
        string topicStr = topicValue.asString();

        if (topicStr == "")
            return (string)"{ \"result\" : \""+ result + "\" , \"message\" : \"topic is null\" }";

        LOG( "executeConsumer starting consumer for " + topicStr);
        consumer->consume( topicStr );
        result = "OK";
        message = "Consumer started!";
    }else if (actionStr == "Get"){
        //DEBUG( "executeConsumer sending getting messages..." );
        result = "OK";
        message = KafkaConsumer::messages;
    }else if (actionStr == "Abort"){
        KafkaConsumer::abort = true;
    }

    return (string)"{ \"result\" : \""+ result + "\" , \"message\" : \"" + message + "\" }";
}

string Kafka::doQuit(){
    string result = (string) "Kafka.doQuit " + attributes[ATT_NAME]+"\n";
    if (producer != nullptr){
        producer->doQuit();
        // delete(producer);
        DELETE_NEBEL( "kafka/producer" , producer );
        result += "PKProducer deleted!\n";
    }
    if (consumer != nullptr){
        consumer->doQuit();
        // delete(consumer);
        DELETE_NEBEL( "kafka/consumer" , consumer );
        result += "PKConsumer deleted!\n";
    }

    return Component::doQuit();
}

int Kafka::getObjectNum(){
    int n = objectsNum;

    // n += producer->getObjectNum(); //They don't use NEBEL_CREATE
    // n += consumer->getObjectNum();
    return n;
}

/*
https://github.com/confluentinc/librdkafka

// $ apt install librdkafka-dev

/home/punksystec/projects/alpha-0/main: symbol lookup error: plugins/K8S-Kafka.so: undefined symbol: _ZN5boost6detail23get_current_thread_dataEv

kubectl port-forward `kubectl get pod -l app=kafka-broker -n kafkanebel -o jsonpath="{.items[0].metadata.name}"` 9092 -n kafkanebel &
kubectl port-forward `kubectl get pod -l app=kafka-broker -n kafka -o jsonpath="{.items[0].metadata.name}"` 9092 -n kafka &
echo "hello world!" | kcat -P -b localhost:9092 -t admintome-test
kcat -C -b localhost:9092 -t admintome-test

error: unable to forward port because pod is not running. Current status=Pending

ALL: producer consumer

CFLAGS=-Wall $(shell pkg-config --cflags glib-2.0 rdkafka)
LDLIBS=$(shell pkg-config --libs glib-2.0 rdkafka)

--- ERROR when 127.0.0.1 kafka-broker is not in /etc/hosts

punksystec@LAPTOP-CPH12905:~/projects/alpha-0$ echo "hello world!" | kcat -P -b localhost:9092 -t admintome-test
%3|1704481844.507|FAIL|rdkafka#producer-1| [thrd:kafka-broker:9092/1]: kafka-broker:9092/1: Failed to resolve 'kafka-broker:9092': Name or service not known (after 2026ms in state CONNECT)
% ERROR: Local: Host resolution failure: kafka-broker:9092/1: Failed to resolve 'kafka-broker:9092': Name or service not known (after 2026ms in state CONNECT)

--- ERROR when prot-forward is not running:

punksystec@LAPTOP-CPH12905:~/projects/alpha-0$ echo "hello world!2" | kcat -P -b localhost:9092 -t admintome-test
echo "hello worldmake clean" | kcat -P -b localhost:9092 -t admintome-test
%3|1704482048.763|FAIL|rdkafka#producer-1| [thrd:localhost:9092/bootstrap]: localhost:9092/bootstrap: Connect to ipv4#127.0.0.1:9092 failed: Connection refused (after 0ms in state CONNECT)
% ERROR: Local: Broker transport failure: localhost:9092/bootstrap: Connect to ipv4#127.0.0.1:9092 failed: Connection refused (after 0ms in state CONNECT)
% ERROR: Local: All broker connections are down: 1/1 brokers are down: terminating




*/


