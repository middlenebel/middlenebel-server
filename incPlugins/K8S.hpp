
#if !defined( K8S_DESKTOP_H )
#define K8S_DESKTOP_H

#include "../inc/Component.hpp"
#include "../incPlugins/K8SDeployment.hpp"
#include "../incPlugins/K8SNamespace.hpp"
#include "../incPlugins/K8SService.hpp"

#define TK_K8S_Image "image"
#define TK_K8S_ContainerPort "containerPort"
#define TK_K8S_Replicas "replicas"

class K8S : public Component {
    private:
        void init();

        //---K8S API
        std::list<string> k8s_versions;
        std::list<K8SDeployment*> k8s_deployments;
        std::list<K8SNamespace*> k8s_namespaces;
        std::list<K8SService*> k8s_services;
        
    public:
        K8S(Component*);
        string getJsonComponent();
        string getJsonDeploys();
        string getJsonNamespaces();
        string getJsonServices();
        string getConfig();
        void parse();
        void parseDeployment();
        void parseNamespace();
        void parseService();
        void parseLabels( Component* k8sElement );
        void parseEnv( K8SDeployment* k8sDep );

        void parseJsonConfig(string json);
        string doPlay();
        string doDestroy();

        string doQuit();
        int getObjectNum();
};

#endif // !defined( K8S_DESKTOP_H )