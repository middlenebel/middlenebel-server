
#if !defined( MySQL_H )
#define MySQL_H

#include "../inc/Component.hpp"

#define TK_TESTCONN "TestConn"
#define TK_INITIALIZE "Initialize"

#define ATTRIB_INITIALIZE "initialize"
#define ATTRIB_INITIALIZE_SCRIPT "initializeScript"

class MySQL : public Component{
    private:
        void init();
    public:
        static int mySQLNum;
        MySQL(Component* parent);

        void parse();
        void parseInitialize();

        string doPlay();
        string doDestroy();
        string doQuit();

        void onLoad();
        string execute( string json );
        string executeTestConn();
        string executeInitialize();
        bool executeScript(sql::Connection *con, string file);
};

#endif // !defined( MySQL_H )