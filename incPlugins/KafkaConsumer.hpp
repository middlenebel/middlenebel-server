#if !defined( KAFKA_CONSUMER_H )
#define KAFKA_CONSUMER_H

#include "../inc/cppkafka/consumer.h"
#include "../inc/cppkafka/configuration.h"

#include "../inc/Component.hpp"

using cppkafka::Consumer;
// using cppkafka::MessageBuilder;

class KafkaConsumer{
    public:
        static string topic;
        static string messages;
        static bool abort;

        KafkaConsumer();
    
        void consume( string topic);

        void doQuit();
};
#endif // !defined( KAFKA_CONSUMER_H )