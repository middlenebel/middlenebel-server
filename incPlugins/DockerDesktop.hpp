
#if !defined( DOCKER_DESKTOP_H )
#define DOCKER_DESKTOP_H

#include "../inc/Component.hpp"

class DockerDesktop : public Component {
    private:

        void init();
        
    public:
        DockerDesktop(Component*);
        void parse();

        // string getJsonComponent();
};

#endif // !defined( DOCKER_DESKTOP_H )