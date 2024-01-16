#if !defined( NEBEL_COMP_H )
#define NEBEL_COMP_H


#include "../inc/Component.hpp"

#include "../incPlugins/Class.hpp"
#include "../incPlugins/System.hpp"

#define TK_Executor "Executor"
#define TK_Class "Class"
#define TK_System "System"
#define TK_PORT_FORWARD "PortForward"

// #define PARSE_ATTRIBUTES() { \
//         if ( key.empty() ){ /*Gets the attribute name*/ \
//                key = token; \
//         } else { \
//             attributes[key]=token; \
//             key = ""; \
//         } \
// }

using namespace std;

class NebelComp : public Component{
    private:
        std::map<string, Component*> objects;

        Component* loadedComponent;

        void init();

        void parseExecutor();
        void parseSubAttributes( string name );
        void parseClass();
        void parseSystem();

    public:
        static int nebelCompNum;

        NebelComp(Component* parent);
        int getObjectNum();

        void parse();

        string doPlay();
        string doDestroy();
        string doQuit();

        string execute( string json );
        string executeSystem(string json);
        string executeClass( string json );

        // void startPortForward();
        // void stopPortForward();
};

#endif // !defined( NEBEL_COMP_H )