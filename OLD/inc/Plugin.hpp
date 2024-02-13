
#if !defined( PLUGIN_H )
#define PLUGIN_H

#include "Component.hpp"

class Plugin:public Syntactical{
    public:
        Component* component;
        virtual void createComponent( Control* control);
};

#endif // !defined( PLUGIN_H )