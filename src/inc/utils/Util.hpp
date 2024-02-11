/*
 * Copyright (c) 2024, David Hernández Tejada
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

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

    static time_t now();
    static string loadFile(string file);
    static void writeFile(string file, string content);
    static void emptyFile(string file);
    //static const string getHttp(string theHost, string thePort, string theTarget);
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
    /* DEBUG(A); // Replication */ \
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

#define DEBUG( A ) ({ \
    Util::middlenebelLog.open("middlenebel-debug.log", std::ios_base::app); \
    Util::middlenebelLog << A << "\n"; \
    Util::middlenebelLog.close();\
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
        TARGET += (*child); \
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
