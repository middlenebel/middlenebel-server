#include "../../incPlugins/KafkaProducer.hpp"

#include <stdexcept>
#include <iostream>

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

#define LOG_COUT( A ) ( cout << A << "\n" )

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
    this->sent++;
    string replaceWith = std::to_string( this->sent );
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

    // string key = to_string( this->sent );
    // builder.key( key );

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
    LOG_COUT("KafkaProducer scheduler");
    string toReplace = "{{num}}";
    std::size_t pos = KafkaProducer::message.find( toReplace );
    //if (pos == std::string::npos) return;
    int num = 0;

    //string brokers = "localhost:9092";
    string brokers = "kafka-broker:9092";
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

    LOG_COUT("Sending max: " + to_string(KafkaProducer::max));
    while ( (((num++) < KafkaProducer::max) || (KafkaProducer::max==0)) && !KafkaProducer::abortSend){
        //DEBUG LOG_COUT("Sending num: " + to_string( num ));
        string newMessage = KafkaProducer::message;
        if (pos!=std::string::npos){        
            string replaceWith = std::to_string(KafkaProducer::sent++);
            newMessage.replace(pos, toReplace.length(), replaceWith);
        }
        //DEBUG LOG_COUT("Payload");
        builder.payload(newMessage);

        //DEBUG LOG_COUT("Produce");
        // Actually produce the message we've built
        cppProducer.produce( builder );

        //DEBUG LOG_COUT("Flush");
        try{
            cppProducer.flush();
        }catch(...){
            LOG_COUT("Exception in Flush");
        }

        //DEBUG LOG_COUT("Sleeping");
        std::this_thread::sleep_for(std::chrono::milliseconds( KafkaProducer::interval ));
        //DEBUG LOG_COUT("End");
    }
    cppProducer.flush();
    return nullptr;
}

void KafkaProducer::autoSend( string topic, string message, int max, int interval ) {
    LOG_COUT( "autoSend init");
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


