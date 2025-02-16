
#if !defined( K8S_SERVICE_H )
#define K8S_SERVICE_H

#include "../inc/Component.hpp"

#define TK_K8S_Service "Service"
#define TK_K8S_Labels "Labels"

class K8SService : public Component {
    private:
        void init();
        
    public:
        K8SService(Component*);
        string getJsonComponent();
        string getYaml();
        string apply();
        string destroy();
};

#endif // !defined( K8S_SERVICE_H )