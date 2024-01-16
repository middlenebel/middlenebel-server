
#if !defined( K8S_NAMESPACE_H )
#define K8S_NAMESPACE_H

#include "../inc/Component.hpp"

#define TK_K8S_CreateNamespaces "CreateNamespaces"

class K8SNamespace : public Component {
    private:
        void init();
        
    public:
        K8SNamespace(Component*);
        string getJsonComponent();
        string getYaml();
        string apply();
        string destroy();
};

#endif // !defined( K8S_NAMESPACE_H )