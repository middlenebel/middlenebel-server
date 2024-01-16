#include "../../incPlugins/KafkaProducer.hpp"

#include <stdexcept>
#include <iostream>
#include <boost/program_options.hpp>
// #include "../../inc/cppkafka/producer.h"
// #include "../../inc/cppkafka/configuration.h"

#include <pthread.h>
#include <chrono>
#include <thread>

using std::string;
using std::exception;
using std::getline;
using std::cin;
using std::cout;
using std::endl;


// using cppkafka::Producer;
using cppkafka::Configuration;
using cppkafka::Topic;
// using cppkafka::MessageBuilder;

int KafkaProducer::max = 10;
int KafkaProducer::sent = 0;
int KafkaProducer::interval = 1000;
string KafkaProducer::message = "Default message";
string KafkaProducer::topic = "purchases";
bool KafkaProducer::abortSend = false;

KafkaProducer::KafkaProducer(){}

void KafkaProducer::send( string topic, string message ) {
    string toReplace = "{{num}}";
    std::size_t pos = message.find( toReplace );
    string replaceWith = std::to_string( this->sent++ );
    if (pos!=std::string::npos){        
        message.replace(pos, toReplace.length(), replaceWith);
    }
    string brokers = "localhost:9092";
    int partition_value = -1;

    // Create a message builder for this topic
    MessageBuilder builder(topic);

    // Get the partition we want to write to. If no partition is provided, this will be
    // an unassigned one
    if (partition_value != -1) {
        builder.partition(partition_value);
    }

    // Construct the configuration
    Configuration config = {
        { "metadata.broker.list", brokers }
    };

    // Create the producer
    cppkafka::Producer cppProducer(config);
    builder.payload(message);

    // Actually produce the message we've built
    cppProducer.produce( builder );
    // }
    
    // Flush all produced messages
    cppProducer.flush();
}

void *producerScheduler (void* caller) {
    LOG("KafkaProducer scheduler");
    string toReplace = "{{num}}";
    std::size_t pos = KafkaProducer::message.find( toReplace );
    //if (pos == std::string::npos) return;
    int num = 0;

    string brokers = "localhost:9092";
    int partition_value = -1;

    // Create a message builder for this topic
    MessageBuilder builder(KafkaProducer::topic);

    // Get the partition we want to write to. If no partition is provided, this will be
    // an unassigned one
    if (partition_value != -1) {
        builder.partition(partition_value);
    }

    // Construct the configuration
    Configuration config = {
        { "metadata.broker.list", brokers }
    };

    // Create the producer
    cppkafka::Producer cppProducer(config);

    LOG("Sending max: " + to_string(KafkaProducer::max));
    while ( (((num++) < KafkaProducer::max) || (KafkaProducer::max==0)) && !KafkaProducer::abortSend){
        //DEBUG LOG("Sending num: " + to_string( num ));
        string newMessage = KafkaProducer::message;
        if (pos!=std::string::npos){        
            string replaceWith = std::to_string(KafkaProducer::sent++);
            newMessage.replace(pos, toReplace.length(), replaceWith);
        }
        //DEBUG LOG("Payload");
        builder.payload(newMessage);

        //DEBUG LOG("Produce");
        // Actually produce the message we've built
        cppProducer.produce( builder );

        //DEBUG LOG("Flush");
        try{
            cppProducer.flush();
        }catch(...){
            LOG("Exception in Flush");
        }

        //DEBUG LOG("Sleeping");
        std::this_thread::sleep_for(std::chrono::milliseconds( KafkaProducer::interval ));
        //DEBUG LOG("End");
    }
    cppProducer.flush();
    return nullptr;
}

void KafkaProducer::autoSend( string topic, string message, int max, int interval ) {
    LOG( "autoSend init");
    KafkaProducer::abortSend = false;
    pthread_t thread1;
    KafkaProducer::max = max;
    KafkaProducer::interval = interval;
    KafkaProducer::sent = 0;
    KafkaProducer::message = message;
    KafkaProducer::topic = topic;
    pthread_create( &thread1, NULL, producerScheduler, (void*)this);
}

void KafkaProducer::doQuit(){
    KafkaProducer::abortSend = true;
}


