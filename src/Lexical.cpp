#include <iostream>
#include <fstream>
#include <vector>

#include "inc/Lexical.hpp"
#include "inc/Core.hpp"
#include "inc/utils/Util.hpp"

using namespace std;

bool lex_debug = false;

Lexical::Lexical(){
    init();
}

void Lexical::load(string file){
    script = file;
    fin.open( file );
    LOG( "Loaded " + file );
}

void Lexical::save(string file, string script){
    std::ofstream output( file, ios_base::out | std::ios::binary);
    if (output.is_open()) {
        output.write(script.data(), script.length());
    }
    output.close();
    LOG( "Saved "+file );
}

void Lexical::init(){
    lexLastToken= "";
    lexLastChar = LEX_UNDEF;
    lexLastType = LEX_UNDEF;
    lexLastCase = LEX_UNDEF;
    prev = LEX_UNDEF;
    isTokenDefined = false;
    flagBack = false;
    isEnd = false;
}

void Lexical::close(){
    fin.close();
}

char Lexical::lexGetChar(){
    lexLastChar = (prev != LEX_UNDEF)? prev : fin.get();
    prev = LEX_UNDEF;
    lexLastCase=LEX_UNDEF;
    lexLastType=LEX_UNDEF;

    LEX_DEBUG("lexLastChar", lexLastChar);
    isTokenDefined = true;
    switch(lexLastChar){
        case '0'...'9': 
            lexLastType = LEX_NUM;
            isTokenDefined = false;
            break;
        case 'a'...'z':
            lexLastType = LEX_CHAR;
            lexLastCase = LEX_LCASE;
            isTokenDefined = false;
            break;
        case 'A'...'Z':
            lexLastType = 'C';
            lexLastCase = LEX_UCASE;
            isTokenDefined = false;
            break;
        case '_': case '-':
            lexLastType = 'C';
            lexLastCase = LEX_UCASE;
            isTokenDefined = false;
            break;
        case '.':               lexLastType = LEX_POINT; break;
        case '\n': case '\r':   lexLastType = LEX_ENDLINE; break;
        case ' ':               lexLastType = LEX_BLANC; break;
        case ':': case ';':     lexLastType = LEX_SEP; break;
        case '/':               lexLastType = LEX_SLASH; break;
        case LEX_QUOTE:              lexLastType = LEX_QUOTE; break;
    }
    return lexLastChar;
}

char Lexical::lexGetToken(){
    if (flagBack){
        flagBack = false;
        return '<';
    }
    // Parse tokens
    while (!isTokenDefined){
        lexGetChar();
        if (!isTokenDefined){
            lexLastToken += lexLastChar;
        }else{ 
            if (lexLastToken.length()>0){
                prev = lexLastChar;
                lexPrevToken = lexLastToken;
                return lexLastChar;
            }else{ //--- Multi-chars operators
                if (lexLastType == LEX_SLASH){
                    lexGetChar();
                    if (lexLastType == LEX_SLASH){ // Skip and return commentary
                        while (lexLastType != LEX_ENDLINE){
                            lexGetChar();
                            lexLastToken += lexLastChar;
                        }
                        lexLastType = LEX_COMMENT;
                        lexPrevToken = lexLastToken;
                        //DEBUG cout << "LEX Commentary: " << lexLastToken;
                        // return LEX_COMMENT;
                        resetToken();
                    } else {
                        lexLastToken = "/";
                        lexPrevToken = lexLastToken;
                        return LEX_SLASH;
                    }
                } else if (lexLastType == LEX_BLANC || lexLastType == LEX_ENDLINE){ // Skip blancs
                    while (lexLastType == LEX_BLANC || lexLastType == LEX_ENDLINE)
                        lexGetChar();
                    prev = lexLastChar;
                } else if (lexLastType == LEX_QUOTE){ // Parse string
                    lexLastToken = "";
                    lexGetChar();
                    while (lexLastType != LEX_QUOTE){
                        lexLastToken += lexLastChar;
                        lexGetChar();
                    }
                    lexLastType = LEX_TYPE_STRING;
                    lexPrevToken = lexLastToken;
                    //DEBUG LOG(  "LEX String: " << lexLastToken << "\n";
                    return LEX_TYPE_STRING; 
                } else if (lexLastType == LEX_POINT){ // End block
                    lexLastToken = ".";
                    lexPrevToken = lexLastToken;
                    return LEX_POINT;
                }
            }
        }
    }
    lexPrevToken = lexLastToken;
    return lexLastType;
}

void Lexical::resetToken(){
    lexLastToken = "";
    isTokenDefined = false;
    prev = LEX_UNDEF;
}

void Lexical::lexBack(){
    lexLastToken = lexPrevToken;
    flagBack = true;
}
