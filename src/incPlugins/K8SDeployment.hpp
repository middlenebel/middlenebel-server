
#if !defined( K8S_DEPLOYMENT_H )
#define K8S_DEPLOYMENT_H

#include <list>

#include "../inc/Component.hpp"

#define TK_K8S_Deployment "Deployment"
#define TK_K8S_Env "Env"

class K8SDeployment : public Component {
    private:
        void init();
        
    public:
        std::map<string, string> env;

        K8SDeployment(Component*);
        string getJsonComponent();
        string getYaml();
        string apply();
        string destroy();
};

#endif // !defined( K8S_DEPLOYMENT_H )