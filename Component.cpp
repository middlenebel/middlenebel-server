
#include <dlfcn.h> // dlopen, RTLD_LAZY, dlsym
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "sys/types.h"
#include "sys/sysinfo.h"

#include "inc/Core.hpp"
#include "inc/Component.hpp"
#include "inc/Control.hpp"
#include "inc/Util.hpp"

using namespace std;

int Component::componentNum = 0;

Component::Component(){
    lex = nullptr;
    parent = nullptr;
    objectsNum = 0;
    init();
}
Component::Component(Component* parent){
    this->parent = parent;
    lex = parent->lex;
    objectsNum = 0;
    init();
}

void Component::init(){
    attributes["className"]="Component";
    attributes["name"]="Component-"+componentNum++;
    synLastChar='\0';
    synLastType=SYN_UNDEF;
    token="";
}

string Component::execute( string actionName){
    LOG(  "Excution not contemplated " + actionName );
    return (string)"{ \"result\" : \"KO\" , \"message\" : \"" + "Excution not contemplated " + actionName + "\" }";
}

void Component::addComponent(Component* component){
    childs.push_back(component);
}

//--------- PARSER

bool Component::readToken(){
    if (lex->fin.eof()) return false; // EOF
    lex->lexGetToken();
    if (lex->lexLastType == LEX_BLANC || 
        lex->lexLastToken.empty() ||
        lex->lexLastToken == " " || 
        lex->lexLastToken == "\n" ||
        lex->lexLastToken == "\r"
        ) lex->lexGetToken();
    token = lex->lexLastToken;
    lex->lexLastToken = "";
    lex->isTokenDefined = false;
    return true;
}

bool Component::isToken(string compareTo){
    //return ( token.compare( compareTo ) == 0 );
    return ( token == compareTo );
}
bool Component::isBlanc(){
    //return (lex->lexLastType == LEX_BLANC);
    return ( token.compare( "" ) == 0 );
}
bool Component::isEndLine(){
    return ( lex->lexLastChar == '\n' || lex->lexLastChar == '\r' );
}
void Component::parse(){
    LOG(  "Parse by " + attributes["name"] );

    while ( readToken() ){
        //DEBUG LOG(  "\nTOKEN: [" << token << "]\n";
        if (isBlanc()) { 
            //DEBUG LOG(  "¬" ); 
            SEG_SHOW_BLANC();
        }
        else if (isToken( TK_POINT )){ return; }
        else if (lex->lexLastType == LEX_COMMENT){    parseComment(); }
        else if (isToken( TK_Control )){    parseControl(); }
        else if (isToken( TK_Debug )){      parseDebug(); }
        else if ( lex->lexLastChar == ':' ){  loadPlugin(token);  
            return;
        }else {
            LOG( "Systax ERROR: " + token );
            return;
        }
    }
    LOG( "End!" );
}
void Component::parseComment(){ //TODO REMOVE THIS. Now parsed in lex.
    LOG( "Comment: " + token );
}

void Component::parseControl(){
     while ( readToken() ){
        LOG( "Control: [" + token );

        if (isToken( TK_start )){         
            //TODO control->start()
            LOG( "Control: start activated" );
            return;
        }else{
            //SYNTAC ERROR
            return;
        }
    }
}

void Component::parseDebug(){
    while ( readToken() ){
        LOG( "Debug: " + token );

        if (isToken( TK_on )){        
            //TODO debug->on() */; 
            LOG("Debug: ON!");
            return;
        }else{
            //SYNTAC ERROR
            return;
        }
    }
}
void Component::loadPlugin(string pluginName){
    string folder ="plugins/";
    string extension = ".so";
    void *libPlugin = loadLibrary(folder + pluginName + extension);

    plugins.push_back( libPlugin );

    LOG("Getting interface...");
    void* (*loadPlugin)(Component*) = (void* (*) (Component*)) dlsym(libPlugin, "loadPlugin");
    void (*parse)(void*) = (void (*) (void*)) dlsym(libPlugin, "parse");

    // if couldn't find the function
    if (loadPlugin == NULL) {
        perror("ERROR\n[ERROR] dlsym");
        printf( "---\n" );
        printf( dlerror() );
        printf( "\n---\n" );
        exit(EXIT_FAILURE);
    }else{
        LOG("Interface Ok!");
    }

    void* instance = loadPlugin(this);
    parse(instance);
}

void* Component::loadLibrary(string librarypath){

    LOG("Library: " + librarypath );

    void* libhandle = dlopen(&librarypath[0], RTLD_LAZY); //functions will be loaded when I try to use them (lazy loading)
    
    // if couldn't find the library
    if (libhandle == NULL) {
        LOG("ERROR!!!" );
        perror("[ERROR] dlopen");
        printf( "---\n" );
        printf( dlerror() );
        printf( "\n---\n" );
        exit(EXIT_FAILURE);
    }else{
        LOG("Ok!" );
    }
    return libhandle;
}

string Component::getAtt(string name){
    string value = this->attributes[name];
    if ((value == "") && (this->parent!=nullptr))
        value = this->parent->getAtt(name);
    return value;
}

string Component::systemCommand(string command, string filenaMeOut, string filenaMeErr){

    command += " > "+filenaMeOut+" 2> "+filenaMeErr;

    system(command.c_str());

    string line;
    string resultCommand = "";

    ifstream file( filenaMeOut );
    if (file.is_open()) {
        while (getline(file, line)) {
            resultCommand += line + "\\n";
        }
        file.close();
    }
    ifstream fileError( filenaMeErr );
    if (fileError.is_open()) {
        while (getline(fileError, line)) {
            resultCommand += line + "\\n";
        }
        fileError.close();
    }
    return resultCommand;
}

string Component::getAtt(string name, string defValue){
    string value = this->attributes[name];
    if (value == ""){
        if (this->parent!=nullptr)
            value = this->parent->getAtt(name, defValue);
        else
            value = defValue;
    }
    return value;
}

// TODO: Maintenance with K8S::getJsonComponent(), Kafca::getJsonComponent()
string Component::getJsonComponent(){
    string components="{";
    components+=JSON_PROPERTY("className", attributes["className"])+",\n";
    components+=JSON_PROPERTY("name", attributes["name"])+",\n";
    components+=JSON_ARRAY( "actions", getJsonActions() )+",\n";
    components+=JSON_ARRAY("attributes", getJsonAttributes() )+",\n";
    components+=JSON_ARRAY( "childs", getJsonChilds() )+"\n";
    components+="}";
    return components;
}

string Component::getJsonActions(){
    string jsonActions = "";
    // Print Strings stored in Vector
    for (long unsigned int i = 0; i < actions.size(); i++){
        jsonActions += "\"" + (string)(actions[i]) + "\"\n";
        jsonActions += (i+1 < actions.size() ? ",\n" : "\n" );
    }
    // for (std::vector<string>::iterator action = actions.begin(); action != actions.end(); ++action){
    //     jsonActions += (string)(action) + "\n";
    //     jsonActions += (std::next(action) != actions.end() ? ",\n" : "\n");
    // }
    return jsonActions;
}
string Component::getJsonAttributes(){
    string json_attributes = "";
    MAP_TO_STRING(attributes, json_attributes);
    return json_attributes;
}

string Component::getJsonChilds(){
    string jsonChilds = "";
    for (std::list<Component*>::iterator child = childs.begin(); child != childs.end(); ++child){
        jsonChilds += (*child)->getJsonComponent()+"\n";
        jsonChilds += (std::next(child) != childs.end() ? ",\n" : "\n");
    }
    return jsonChilds;
}

string Component::getJsonLabels(){
     //DEBUG     
     LOG( "K8S Labels..." ); //TODO why not core directly. 
     string json_labels = "";
     MAP_TO_STRING(labels, json_labels);
     return json_labels;
}

//TODO
string Component::getYamlLabels(){
     return (string)
"  labels:\n";
}

string Component::doPlay(){
    string result = "";
    for (std::list<Component*>::iterator child = childs.begin(); child != childs.end(); ++child){
        result += (*child)->doPlay();
    }
    return result;
}

string Component::doDestroy(){
    string result = "";
    for (std::list<Component*>::iterator child = childs.begin(); child != childs.end(); ++child){
        result += (*child)->doDestroy();
    }
    return result;
}
string Component::doQuit(){
    LOG( attributes["name"] + ".doQuit! Childs: " + to_string(childs.size()) + " Plugins: "+ to_string(plugins.size()) );
    string result = (string) "doQuit " + attributes["name"]+"\n";
    int n=0;
    for (std::list<Component*>::iterator child = childs.begin(); child != childs.end(); ++child){
        result += (*child)->doQuit();
        DELETE_NEBEL( attributes["name"]+"/child-"+to_string(n++) , (*child) );
    }
    childs.clear();
    for (std::list<void*>::iterator plugin = plugins.begin(); plugin != plugins.end(); ++plugin){
        result += "Remove library!\n";
        dlclose( *plugin );
    }
    plugins.clear();

    return result;
}

Component* Component::getRootComponent(Component* component){
    if (component == nullptr) return nullptr;
    if (component->parent == nullptr){
        return (Core*) component;
    }
    return getRootComponent(component->parent);
}

int Component::getObjectNum(){
    int n = objectsNum;
    for (std::list<Component*>::iterator child = childs.begin(); child != childs.end(); ++child){
        n += (*child)->getObjectNum();
    }
    return n;
}

