
#if !defined( COMPONENT_H )
#define COMPONENT_H

#include <list>
#include <memory>
#include <vector>
#include <map>

#include "Lexical.hpp"
#include "Util.hpp"

#define ATT_PORT    "port"
#define ATT_APP     "app"
#define ATT_NAMESPACE "namespace"

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
    /*LOG_FILE((string)"CREATING "+to_string(Util::objectsNum)+" "+NAME, "memory.log");*/ \
    A = new B; \
    objectsNum++; \
}

#define DELETE_NEBEL( NAME, A ) { \
    /*LOG_FILE((string)"DELETING "+to_string(Util::objectsNum)+" "+NAME, "memory.log");*/ \
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

#define PARSE_ATTRIBUTES() { \
        if ( key.empty() ){ /*Gets the attribute name*/ \
               key = token; \
        } else { \
            attributes[key]=token; \
            key = ""; \
        } \
}

// class Core;
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

        // Core* core;
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

        virtual string systemCommand(string command, 
            string filenaMeOut = "system_command.out",
            string filenaMeErr = "system_error.out" );

        virtual void parse();
        virtual string doPlay();
        virtual string doDestroy();
        virtual string doQuit();
        virtual void startPortForward();
        void startPortForward(string app, string nameSpace, string port);
        virtual void stopPortForward();
};

#endif // !defined( COMPONENT_H )