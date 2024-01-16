
#if !defined( COMPONENT_H )
#define COMPONENT_H

#include <list>
#include <memory>
#include <vector>
#include <map>

#include "Lexical.hpp"
#include "Util.hpp"

#define TK_Control "Control"
#define TK_start "start"
#define TK_Debug "Debug"
#define TK_on "on"
#define TK_POINT "."
//#define TK_Blanc "B"

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

// class Core;
using namespace std;

class Component : public enable_shared_from_this<Component>{
    private:
        void parseControl();
        void parseDebug();
        
        void* loadLibrary(string librarypath);
        void parseComment();
        
    public:
        static int componentNum; //Index of components created, used as identifier.
        int objectsNum; //Dynamics objects created/destroyed, for memory control

        // Core* core;
        Component* parent;
        std::list<Component*> childs;
        std::list<void*> plugins;
        std::vector<string> actions;

        std::map<string, string> attributes;
        std::map<string, string> labels;
        std::map<string, Component*> executors;

        Component();
        Component(Component* parent);

        virtual int getObjectNum();

        virtual ~Component() {/* stuff */};

        void loadPlugin(string pluginName);
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
};

#endif // !defined( COMPONENT_H )