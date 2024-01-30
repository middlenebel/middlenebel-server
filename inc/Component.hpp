
#if !defined( COMPONENT_H )
#define COMPONENT_H

#include <list>
#include <memory>
#include <vector>
#include <map>

#include "Util.hpp"
#include "Util.hpp"
#include "Lexical.hpp"
#include "PortForward.hpp"
#include "PortForward.hpp"

#define ATT_NAMESPACE "namespace"
#define ATT_NAME    "name"
#define ATT_APP     "app"
#define ATT_PORT    "port"
#define ATT_CLASSNAME "className"
#define ATT_NAME    "name"
#define ATT_APP     "app"
#define ATT_PORT    "port"
#define ATT_CLASSNAME "className"

#define TK_Control  "Control"
#define TK_start    "start"
#define TK_Debug    "Debug"
#define TK_on       "on"
#define TK_POINT    "."
//#define TK_Blanc "B"

#define TK_PORT_FORWARD "PortForward"

#define SYN_UNDEF '\0'
#define SYN_IDENT 'I'
#define SYN_NUM 'N'
#define SYN_STR 'S'
#define SYN_CHAR 'C'

#define SYN_TODO(A) (cout<<"SYN_TODO "<<A)

//#define SEG_SHOW_BLANC() (LOG( "¬" ))
// Disabled show whitespaces:
#define SEG_SHOW_BLANC() ({;})

#define CREATE_NEBEL( NAME, A, B ) { \
    LOG_FILE((string)"CREATING + "+NAME, "memory.log"); \
    LOG_FILE((string)"CREATING + "+NAME, "memory.log"); \
    A = new B; \
    objectsNum++; \
}
#define CREATE_NEBEL_PLUGIN( NAME, A, B ) { \
    LOG_FILE((string)"CREATING + "+NAME, "memory.log"); \
    A = new B; \
}
#define CREATE_NEBEL_PLUGIN( NAME, A, B ) { \
    LOG_FILE((string)"CREATING + "+NAME, "memory.log"); \
    A = new B; \
}

#define DELETE_NEBEL( NAME, A ) { \
    LOG_FILE((string)"DELETING - "+NAME, "memory.log"); \
    LOG_FILE((string)"DELETING - "+NAME, "memory.log"); \
    objectsNum--; \
    if (A == nullptr ){ \
        LOG((string)"ERROR DELETE NULL PTR "+NAME); \
    }else{  \
        try{ \
            delete(A); \
        }catch(...){ \
            LOG((string)"Exception deleting "+NAME); \
        } \
    } \
    A=nullptr; \
}

#define RETURN_EXECUTE_OK (string)"{ \"result\" : \"OK\" , \"message\" : \"Executed sucessfully!\" }" 
#define RETURN_EXECUTE_KO (string)"{ \"result\" : \"KO\" , \"message\" : \"ERROR Executing!\" }"

#define DUMP_ATT(K,T) { \
    LOG("Attribute "<< K <<" = "<< T); \
}

/*Gets the attribute name*/ 
#define PARSE_ATT_KEY_TOKEN(A,K,T) { \
        if ( K.empty() ){ K = T; } \
        else { A[K]=T; K = ""; /*DUMP_ATT(K,T);*/ } \
}

using namespace std;

class Component : public enable_shared_from_this<Component>{
    private:
        bool portForwardInited;

        void parseControl();
        void parseDebug();
        
        void* loadLibrary(string librarypath);
        void parseComment();

    protected:
        virtual void parsePortForward();
        
    public:
        static int componentNum; //Index of components created, used as identifier.
        int objectsNum; //Dynamics objects created/destroyed, for memory control

        bool portForward;

        Component* parent;
        std::list<Component*> childs;
        std::list<void*> plugins;
        // std::vector<string> actions;
        std::map<string, string> actions;
        std::map<string, string> attributes;
        std::map<string, string> labels;
        std::map<string, Component*> executors;

        Component();
        Component(Component* parent);

        virtual int getObjectNum();

        virtual ~Component() {/* stuff */};

        Component* loadPlugin(string pluginName);
        Component* getRootComponent(Component* component);

        void addComponent(Component* component);
 
        virtual string execute( string actionName);

        virtual string getJsonComponent();
        virtual string getJsonChilds();
        virtual string getJsonActions();
        virtual string getJsonAttributes();
                
        virtual string getJsonLabels();
        virtual string getYamlLabels();

        void init(); //TODO do virtual.

        //--- Parser
        Lexical* lex;    
        string token;    
        bool readToken();
        bool isToken(string compareTo);
        bool isBlanc();
        bool isEndLine();

        char synLastChar;
        char synLastType;

        string getAtt(string name);
        string getAtt(string name, string defValue);

        static string systemCommand(string command, 
            string filenaMeOut = "system_command.out",
            string filenaMeErr = "system_error.out" );

        virtual void parse();
        virtual string doPlay();
        virtual string doDestroy();
        virtual string doQuit();

        virtual void startPortForward(PortForward* pf);
        // virtual void startPortForward(string app, string nameSpace, string port);
        virtual void stopPortForward(PortForward* pf);

        virtual void startPortForward(PortForward* pf);
        // virtual void startPortForward(string app, string nameSpace, string port);
        virtual void stopPortForward(PortForward* pf);
};

#endif // !defined( COMPONENT_H )