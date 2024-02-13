
#if !defined( SHARED_H )
#define SHARED_H

#include "../inc/Component.hpp"

class Shared : public Component{
    public:
        Shared(Component* parent);
        void onLoad();
        string execute( string json );
};

#endif // !defined( SHARED_H )