#if !defined( PLATFORM_H )
#define PLATFORM_H

#include "../inc/Component.hpp"

#define TK_Platform "Platform"
#define TK_Server "Server"
#define TK_Cluster "Cluster"
#define TK_DockerDesktop "DockerDesktop"
#define TK_K8S "K8S"
#define TK_DBase "DBase"

#define TK_NebelComp "NebelComp"
#define TK_Kafka "K8S-Kafka"
#define TK_MySQL "K8S-MySQL"

class Platform : public Component{
    private:
        void init();

        void parseDockerDesktop();
        void parseK8S();
        void parseServer();
        void parseDBase();

    public:
        static int platformNum;
        Platform(Component* parent);

        void parse();
        void Server();
        void DBase();

        // string getJsonComponent();
};
#endif // !defined( PLATFORM_H )