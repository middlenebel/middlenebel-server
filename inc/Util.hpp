#if !defined( UTIL_H )
#define UTIL_H

#include <iostream>
#include <fstream>

using namespace std;

#define MIDDLENEBEL_EXIT_FAILURE "EXIT_FAILURE"

class Util{
    public:
    static ofstream middlenebelLog;
    static ofstream middlenebelLogFile;

    static int objectsNum;
    static int objectsSize;

    static time_t now();
    static string loadFile(string file);
    static const string getHttp(string theHost, string thePort, string theTarget);
    static bool endsWith(string data, string sufix);
};

#define LOG_INIT( A ) ({ \
    Util::middlenebelLog.open("middlenebel.log"); \
    Util::middlenebelLog << A << "\n"; \
    Util::middlenebelLog.close(); \
})

#define LOG( A ) ({ \
    Util::middlenebelLog.open("middlenebel.log", std::ios_base::app); \
    Util::middlenebelLog << A << "\n"; \
    Util::middlenebelLog.close();\
})

#define LOG_INIT_FILE( A , FILE ) ({ \
    Util::middlenebelLogFile.open(FILE); \
    Util::middlenebelLogFile << A << "\n"; \
    Util::middlenebelLogFile.close(); \
})

#define LOG_FILE( A , FILE ) ({ \
    Util::middlenebelLogFile.open(FILE, std::ios_base::app); \
    Util::middlenebelLogFile << A << "\n"; \
    Util::middlenebelLogFile.close();\
})

#define JSON_PROPERTY( PROP, VALUE ) ( (string)"\"" + PROP + "\":\"" + VALUE + "\"" )
#define JSON_ARRAY( PROP, CONTENT ) ( (string)"\"" + PROP + "\":[" + CONTENT + "]" )
#define JSON_OBJECT( PROP, CONTENT ) ( (string)"\"" + PROP + "\":{" + CONTENT + "}" )

#define JSON_ARRAY_TO_LIST(values, list) { \
    for ( unsigned int index = 0; index < values.size(); ++index ) \
        list.push_back(values[index].asString()); \
}

#define LIST_TO_STRING(LIST, TARGET) { \
    for (std::list<string>::iterator child = LIST.begin(); child != LIST.end(); ++child){ \
        TARGET += (string)"\"" + (*child) + "\""; \
        TARGET += (std::next(child) != LIST.end() ? ",\n" : "\n");\
    } \
}

#define MAP_TO_STRING(MAP, TARGET) { \
    for (std::map<string,string>::iterator child = MAP.begin(); child != MAP.end(); ++child){ \
        TARGET += (string)"[\"" + (child->first) + "\""; \
        TARGET += (string)" , \"" + (child->second) + "\"]"; \
        TARGET += (std::next(child) != MAP.end() ? ",\n" : "\n");\
    } \
}

#define LOG_ATTRIBUTES(){ \
    for (std::map<string,string>::iterator item = attributes.begin(); item != attributes.end(); ++item){ \
        LOG( (string)"[ " + (item->first) + " = "+ (item->second) + "]"); \
    } \
}
// #define CREATE_NEBEL( NAME, A, B ) { \
//     LOG_FILE((string)"CREATING "+to_string(Util::objectsNum)+" "+NAME, "memory.log"); \
//     A = new B; \
//     Util::objectsNum++; \
//     Util::objectsSize += sizeof( A ); \
// }

// #define DELETE_NEBEL( NAME, A ) { \
//     LOG_FILE((string)"DELETING "+to_string(Util::objectsNum)+" "+NAME, "memory.log"); \
//     Util::objectsNum--; \
//     Util::objectsSize -= sizeof( A ); \
//     if (A == nullptr ){ \
//         LOG((string)"ERROR DELETE NULL PTR "+NAME); \
//     }else{  \
//         try{ \
//             delete(A); \
//         }catch(...){ \
//             LOG((string)"Exception deleting "+NAME); \
//         } \
//     } \
//     A=nullptr; \
// }

#endif // !defined( UTIL_H )
