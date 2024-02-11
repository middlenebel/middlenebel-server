#if !defined( KAFKA_CONSUMER_H )
#define KAFKA_CONSUMER_H

#include "../../third_party/cppkafka/include/cppkafka/consumer.h"
#include "../../third_party/cppkafka/include/cppkafka/configuration.h"

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