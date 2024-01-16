#if !defined( KAFKA_PRODUCER_H )
#define KAFKA_PRODUCER_H

#include "../inc/cppkafka/producer.h"
#include "../inc/cppkafka/configuration.h"

#include "../inc/Component.hpp"

using cppkafka::Producer;
using cppkafka::MessageBuilder;

class KafkaProducer{
    public:
        static int max;
        static int sent;
        static int interval;
        static string message;
        static string topic;
        static bool abortSend;

        KafkaProducer();
        // Producer* cppProducer;
        // MessageBuilder* builder;
        // Configuration config;
    
        void send( string topic, string message);
        void autoSend( string topic, string message, int max, int interval);

        void doQuit();
};
#endif // !defined( KAFKA_PRODUCER_H )