#if !defined( PORTFORWARD_H )
#define PORTFORWARD_H

class Component;

struct PortForward{
    std::string id;
    std::string appName;
    std::string port;
    std::string nameSpace;
    Component* component;
};
// typedef struct _PortForward PortForward;

#endif // !defined( PORTFORWARD_H )