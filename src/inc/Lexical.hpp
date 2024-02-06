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

#if !defined( LEXICAL_H )
#define LEXICAL_H

#include <iostream>
#include <fstream>

#include "utils/Util.hpp"

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