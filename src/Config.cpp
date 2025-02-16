
#include <cstdlib>
#include <iostream>
#include <string>

#include "./inc/Core.hpp"
#include "./inc/Component.hpp"
#include "./inc/utils/Util.hpp"
#include "./inc/Config.hpp"

#define LOG_COUT( A ) ( cout << A << "\n" )

//Config::Config(Component* parent):Component(parent){
Config::Config(string file){
    parent = nullptr;
    config = this;
    this->lex = new Lexical();
    init();
    loadConfig(file);
    //  parent->addComponent(this); // Special case Config is independent.
}

// Subconfigs
Config::Config(Lexical* lex, Component* parent){
    parent = parent;
    config = this;
    this->lex = lex;
    init();
}

void Config::init(){
     attributes[ATT_CLASSNAME]="Config";
     attributes[ATT_NAME]="Config";

     //if (parent->attributes[ATT_NAME] != "Config"){ 
    if (parent == nullptr){ // global config values: 'useConfig', etc...
        // This is not a sub set of config. Sets the main values in the root Config container.
        attributes[ATT_USE_CONFIG]=CFG_DEFAULT;
     }
}

string Config::cfg(string key, string defValue){
    string useConfig = attributes[ATT_USE_CONFIG];
    if (useConfig.empty()){
        LOG_COUT("ERROR useConfig undefined! Using 'Default' instead.");
        useConfig = "Default";
    }
    Config* realConfig = configurations[ useConfig ];
    if (realConfig == nullptr){
        LOG_COUT("WARNING! Configuration not defined: " + useConfig + "Using global.");
        realConfig = this;
    }
    string value = realConfig->attributes[ key ];
    if (value == "") value = defValue;
    return value;
}

void Config::parse(){
    LOG_COUT( "Parse by Config..." );
    string key="", value="";
    while ( readToken() ){
        //DEBUG LOG_COUT( "\Config TOKEN: " << token << "\n";

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; } //TODO error?
        else if (isToken( TK_CONFIG )){ parseConfig(); }
        else PARSE_ATT_KEY_TOKEN(attributes, key, token);
    }
    LOG_COUT( "Using configuration: " + configurations[ attributes[ATT_USE_CONFIG] ]->attributes[ATT_NAME]);
    if ( IS_CONFIG( ATT_DUMP_CONFIG, CFG_TRUE)) DUMP_CONFIGURATION;
}

void Config::parseConfig(){
    //DEBUG LOG_COUT( "parseConfigs..." );
    bool parsedName = false;
    string key="", value="";
    Config* newConfig;
    int skipTokens = 0;
    while ( readToken() ){
        //DEBUG LOG_COUT( "\Config TOKEN: " << token << "\n";

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        else if ( !parsedName ){ // Gets the name of configuration
            //DEBUG: LOG_COUT( "Config: " + token );
            CREATE_NEBEL( " <<< config >>> ", newConfig, Config(lex, this));
            configurations[token]=newConfig;
            newConfig->attributes[ATT_NAME]=token;
            parsedName = true;
        }
        else{
            if (key.empty() && (token == ATT_USE_CONFIG)){
                LOG_COUT("ERROR in Config.nebel. 'useConfig' only can be setted in the global area.");
                skipTokens = 2; // Skip "KEY VALUE"
            }
            if (skipTokens == 0)
                PARSE_ATT_KEY_TOKEN( newConfig->attributes, key, token )
            else skipTokens--;
        }
    }
}

void Config::loadConfig(string fileName){
    this->fileName = fileName;
    lex->load(fileName);

    if (Util::endsWith(fileName , ".nebel")){
        parse();
    }
    lex->close();
}

void Config::saveConfig(string fileName, string script){
    lex->save(fileName, script);
}

int Config::getObjectNum(){
    int n = objectsNum;
    for (std::map<string,Config*>::iterator cfgItem = configurations.begin(); cfgItem != configurations.end(); ++cfgItem){
        n += cfgItem->second->getObjectNum();
    }
    return n;
}

string Config::doQuit(){
    string result = (string) "doQuit " + attributes[ATT_NAME]+"\n";
    LOG_COUT("DEBUG Config::doQuit " + configurations.size());
    for (std::map<string,Config*>::iterator cfgItem = configurations.begin(); cfgItem != configurations.end(); ++cfgItem){
        DELETE_NEBEL(" <<< config >>> ", cfgItem->second);
        result += "Deleted config "+cfgItem->first+"\n";
    }
    return result;
}