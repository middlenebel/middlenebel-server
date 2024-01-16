
#if !defined( LEXICAL_H )
#define LEXICAL_H

#include <iostream>
#include <fstream>

#include "Util.hpp"

#define LEX_DEBUG( A, B ) ({\
    if (lex_debug) cout << "LEX-DEBUG: " << A << " : " << B << "\n";\
})

#define LEX_UNDEF '\0'
#define LEX_NUM 'n'
#define LEX_CHAR 'C'
#define LEX_UCASE 2
#define LEX_LCASE 1
#define LEX_TYPE_STRING 'S'

#define LEX_POINT '.'
#define LEX_SPACE ' '
#define LEX_SEP '_'
#define LEX_BLANC 'B'
#define LEX_SLASH '/'
#define LEX_ENDLINE '\n'
#define LEX_COMMENT '#'
#define LEX_QUOTE '\''

using namespace std;

// class Core;

class Lexical{
    private:
        // Core* core;
        int lexPosition;
        void init();
        
    public:
        string script;
        ifstream fin;
        string lexLastToken;
        string lexPrevToken;
        char lexLastChar;
        char lexLastType;
        char lexLastCase;
        char prev;
        bool flagBack;
        bool isTokenDefined;
        bool isEnd;

        Lexical();
        void load(string);
        void save(string file, string script);
        char lexGetChar();
        char lexGetToken();
        void lexBack();
        void skipBkancs();
        void resetToken();
        void close();
        // void setCore(Core*);
};

#endif // !defined( LEXICAL_H )