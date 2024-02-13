
#if !defined( K8S_LABEL_H )
#define K8S_LABEL_H

#include "../inc/Component.hpp"

class K8SLabel: public Component{
    public:
        K8SLabel(Component* parent);
        string getJsonLabels();
        string getYamlLabels();
};

#endif // !defined( K8S_LABEL_H )