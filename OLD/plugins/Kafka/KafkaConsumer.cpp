#include "../../incPlugins/KafkaConsumer.hpp"

#include <stdexcept>
#include <iostream>
#include <boost/program_options.hpp>

#include <pthread.h>
#include <chrono>
#include <thread>

using std::string;
using std::exception;
using std::getline;
using std::cin;
using std::cout;
using std::endl;

using cppkafka::Configuration;
using cppkafka::Topic;
using cppkafka::Message;
using cppkafka::TopicPartitionList;

string KafkaConsumer::topic = "purchases";
string KafkaConsumer::messages;
bool KafkaConsumer::abort = false;

KafkaConsumer::KafkaConsumer(){}

void *consumerScheduler (void* caller) {
    LOG("KafkaConsumer scheduler");

    // Construct the configuration
    string brokers = "localhost:9092";
    Configuration config = {
        { "metadata.broker.list", brokers },
        { "group.id", "1" },
        // Disable auto commit
        { "enable.auto.commit", false }
    };

    // Create the consumer
    Consumer consumer(config);

    // // Print the assigned partitions on assignment
    consumer.set_assignment_callback([](const TopicPartitionList& partitions) {
        //cout << "Got assigned: " << partitions << endl;
        LOG( "Got assigned!" );
    });

    // // Print the revoked partitions on revocation
    consumer.set_revocation_callback([](const TopicPartitionList& partitions) {
        //cout << "Got revoked: " << partitions << endl;
        LOG( "Got revoked!" );
    });

    LOG( "Subscribing to topic " + KafkaConsumer::topic );

    // Subscribe to the topic
    consumer.subscribe({ KafkaConsumer::topic });

    LOG( "Consuming messages from topic " + KafkaConsumer::topic );

    // Now read lines and write them into kafka
    while ( !KafkaConsumer::abort ) {
        // Try to consume a message
        Message msg = consumer.poll();
        //DEBUG( "Polled!" );

        if (msg) {
            //DEBUG LOG( "Message!" );
            // If we managed to get a message
            if (msg.get_error()) {
                // Ignore EOF notifications from rdkafka
                if (!msg.is_eof()) {
                    LOG( "[+] Received error notification: " + msg.get_error().to_string() );
                }
            }
            else {
                string msgLine = "";
                // Print the key (if any)
                if (msg.get_key()) {
                    msgLine = (string) msg.get_key() + " -> ";
                }
                // Print the payload
                msgLine+= (string) msg.get_payload() + "\\n";

                DEBUG("Message " + (string) msg.get_payload());

                // Now commit the message
                consumer.commit(msg);

                KafkaConsumer::messages += msgLine;
            }
        }
    }
    return nullptr;
}

void KafkaConsumer::consume( string topic){
    KafkaConsumer::topic = topic;
    KafkaConsumer::abort = false;
    pthread_t thread1;
    pthread_create( &thread1, NULL, consumerScheduler, (void*)this);
}

void KafkaConsumer::doQuit(){
    KafkaConsumer::abort = true;
}



