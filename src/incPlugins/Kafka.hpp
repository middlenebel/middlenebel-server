#if !defined( KAFKA_H )
#define KAFKA_H

#include "../inc/Component.hpp"

#include "../incPlugins/KafkaProducer.hpp"
#include "../incPlugins/KafkaConsumer.hpp"

#define TK_Kafka "K8S-Kafka"
#define TK_PORT_FORWARD "PortForward" 
#define TK_KAFKA_PRODUCER "KProducer"
#define TK_KAFKA_CONSUMER "KConsumer"

using namespace std;

class Kafka : public Component{
    private:
        KafkaProducer* producer;
        KafkaConsumer* consumer;

        bool portForwardInited;

        void init();

        void parseKafka();

    public:
        static int kafkaNum;
        bool portForward;
        Kafka(Component* parent);

        void parse();

        void doPlay(std::list<std::string> *cmds);
        void doDestroy(std::list<std::string> *cmds);
        
        string execute( string actionName );
        string executeProducer(string json);
        string executeConsumer( string json );

        // void startPortForward();
        // void stopPortForward();

        string doQuit();
        int getObjectNum();
};
#endif // !defined( KAFKA_H )