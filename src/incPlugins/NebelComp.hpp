#if !defined( NEBEL_COMP_H )
#define NEBEL_COMP_H


#include "../inc/Component.hpp"

#include "../incPlugins/Class.hpp"
#include "../incPlugins/System.hpp"

#define TK_Executor "Executor"
#define TK_Class "Class"
#define TK_System "System"
#define TK_PORT_FORWARD "PortForward"

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

        void doPlay(std::list<std::string> *cmds);
        void doDestroy(std::list<std::string> *cmds);
        string doQuit();

        string execute( string json );
        string executeSystem(string json);
        string executeClass( string json );

        // void startPortForward();
        // void stopPortForward();
};

#endif // !defined( NEBEL_COMP_H )