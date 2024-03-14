
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>

#include <future>
#include <thread>
#include <utility>

#include "inc/Core.hpp"
#include "inc/utils/Util.hpp"

Core* Core::coreInstance = nullptr;

namespace fs = std::filesystem;

Core* Core::getInstance(){
    if (coreInstance==nullptr){
        coreInstance = new Core();
    }
    return coreInstance;
}
Core::Core(){
    this->lex = new Lexical();
    this->config = nullptr;
    parent = nullptr;
    quit = false;
    objectsNum = 0;
    config = nullptr;
    init();
}
void Core::init(){
    attributes[ATT_CLASSNAME]="Core";
    attributes[ATT_NAME]="Core";
    synLastChar='\0';
    synLastType=SYN_UNDEF;
    token="";
    oFileName = "./script.nebel";
    newFileName = "";
    portForwardingRequested = "";

    executors["test"] = this;
}
void Core::setConfig(Config* config){
    this->config = config;
}
void Core::setReloadPromise( std::promise<int>&& reloadPromise ){
    this->reloadPromise = std::move(reloadPromise);
}

void Core::load(string fileName){
    //this->oFileName = fileName; //TODO Save it only if parse ok (?)
    fileName = this->oFileName.value(); //Allways work with the same file
    cout <<"DEBUG fileName " << fileName <<"\n";
    bool loaded = lex->load(fileName);
  
    if (loaded && Util::endsWith(fileName , ".nebel")){
        LOG("Core:loaded " << fileName);
        if ( IS_NOT_CONFIG( ATT_DISABLE_PARSE, CFG_TRUE ) ){
            parse();
            LOG("Core: Parsing complete!");
            LOG("-----------------------");
            if (portForwards.size() > 0){
                executors[ EXECUTOR_PORTFORWARD ] = this; 
                this->actions[ EXECUTOR_PORTFORWARD ] = EXECUTOR_PORTFORWARD;
            }
        }else{
            LOG("Core: Parse is disabled by configuration! Skipping parse...");
        }
    }
    lex->close();
}

void Core::save(string fileName, string script){
    //Allways work with the same file
    //lex->save(fileName, script);
    lex->save( oFileName.value(), script );
    LOG("Core:Saved!");
}

//====================/ HTTP REQUESTS \======================
#define RELOAD_TRUE true
#define RELOAD_FALSE false
bool Core::getStatus(const Request &req, Response &res){ 
    res.set_content("Up!", "text/plain"); 
    return RELOAD_FALSE;
}
bool Core::getComponents(const Request &req, Response &res){
    string json = "[" + getJsonComponent() + "]";
    //response.body() = json; 
    res.set_content(json, "application/json"); 
    return RELOAD_FALSE;
}
bool Core::getPlay(const Request &req, Response &res){ 
    cout << "Core::getPlay\n";
    commandList.clear();
    doPlay();

    string commands;
    LIST_TO_STRING( commandList, commands);
    string json = "{\"commandList\":["+ commands + "]}"; //TODO format response

    res.set_content(json, "application/json");  
    return RELOAD_FALSE;
}
bool Core::getDestroy(const Request &req, Response &res){ 
    LOG("Destroying platform in target systems...");
    string json = "["+ doDestroy() + "]";
    //DEPRECATED string json = "{ \"result\" : \"OK\" , \"message\" : \"Destroy success!\" }";
    res.set_content(json, "application/json"); 
    reloadPromise.set_value(1);
    return RELOAD_TRUE;
}
bool Core::getQuit(const Request &req, Response &res){ 
    LOG("Close server requested!!!");
    string message = "Quiting...!";
    string json = "{ \"result\" : \"OK\" , \"message\" : \"Quit success!\" }";
    res.set_content(json, "application/json"); 
    quit = true;
    return RELOAD_FALSE;
}   
bool Core::getReload(const Request &req, Response &res){ 
    LOG("Reload requested!!!");
    string json = "{ \"result\" : \"OK\" , \"message\" : \"Reload success!\" }";
    res.set_content(json, "application/json");
    reloadPromise.set_value(1);
    return RELOAD_TRUE;
}
//TODO Save in proxy only
bool Core::postSaveScript(const Request &req, Response &res){ 
    LOG("Save requested!!!");
    string script = req.body;
    save( oFileName.value(), script );
    cout << "Saved script to " << oFileName.value() <<"\n";
    string json = "{ \"result\" : \"OK\" , \"message\" : \"Save success!\" }";
    res.set_content(json, "application/json"); 
    reloadPromise.set_value(1);
    return RELOAD_TRUE;
}

bool Core::getLog(const Request &req, Response &res){ 
    string logContent = Util::loadFile( "middlenebel.log" );
    string json = 
            (string) "{ \"result\" : \"OK\" , \"message\" : \""+ logContent +"\" }";
    res.set_content(json, "application/json");
    return RELOAD_FALSE;
}
bool Core::getClearLog(const Request &req, Response &res){ 
    Util::emptyFile(FILE_MIDDLENEBEL_LOG);
    string logContent = "Log cleaned!";
    string json = 
        (string) "{ \"result\" : \"OK\" , \"message\" : \""+ logContent +"\" }";
    res.set_content(json, "application/json");
    return RELOAD_FALSE;
}
bool Core::getExecuteAction(const Request &req, Response &res){ 
    string json = doExecuteAction( req.body );
    res.set_content(json, "application/json");
    if (this->newFileName != "")
        return RELOAD_TRUE;
    return RELOAD_FALSE;
}
//====================\ END HTTP REQUESTS /====================

string Core::getJsonComponent(){
    string components="{";
    components+=JSON_PROPERTY( ATT_CLASSNAME , attributes[ATT_CLASSNAME])+",\n";
    components+=JSON_PROPERTY( ATT_NAME , attributes[ATT_NAME] )+",\n";
    components+=JSON_ARRAY( "childs" , getJsonChilds() )+",\n";
    components+=JSON_ARRAY( "portForwards" , getJsonPortForwards() )+",\n";
    components+=JSON_PROPERTY( "script" , Util::loadFile( lex->script ) )+"\n";
    components+="}";
    return components;
}

string Core::getJsonPortForwards(){
    string result = "";
    for (std::map<string,PortForward*>::iterator child = portForwards.begin(); child != portForwards.end(); ++child){
        result += "{";
        result += (string)"\"id\":\"" + (child->second->id) + "\",";
        result += (string)"\"appName\":\"" + (child->second->appName) + "\",";
        result += (string)"\"port\":\"" + (child->second->port) + "\",";
        result += (string)"\"nameSpace\":\"" + (child->second->nameSpace) + "\"";
        result += "}";
        result += (std::next(child) != portForwards.end() ? ",\n" : "\n");
    }
    return result;
}

string Core::doExecuteAction(string json){
    string result = "KO";
    string message = "Server ERROR!";

    Json::Value value;
    Json::Reader reader; //std::stringstream sstr(json);
    bool isOk = reader.parse( json, value ); // sstr >> api;
    if (isOk){
        const Json::Value actionValue = value["action"];
        string actionStr = actionValue.asString();

        result = "OK";
        message = (string)"Action " + actionStr + " executed!";

        Component* executor = executors[ actionStr ];
        if (executor != nullptr ){
            //DEBUG LOG(" Core calling executor "+executor->attributes[ATT_NAME] );
            string response = RETURN_EXECUTE_KO;
            try{
                response = executor->execute( json );
            }catch(const std::exception& ex){
                LOG("Exception "<<ex.what());
                LOG("Exception in doExecuteAction! calling to " << executor->attributes[ATT_NAME]);
            }
            return response;
        }else{
            LOG("Executor not found for " << actionStr);
        }
    }

    return (string) "{ \"result\" : \""+ result +"\" , \"message\" : \""+ message +"\" }";
}
string Core::execute( string json){
    LOG("Core execute ");

    Json::Value actionJson;
    Json::Reader reader; 
    bool isOk = reader.parse( json, actionJson );
    if (!isOk){
        LOG( "Error parsing actions json! " + json);
        return (string)"{ \"result\" : \"KO\" , \"message\" : \"Error parsing actions json!\" }";
    }

    const Json::Value actionValue = actionJson["action"]; 
    string actionStr = actionValue.asString();

    if (actionStr==EXECUTOR_PORTFORWARD){
        portForwardingRequested = "true";
        return startAllPortforwards();
        LOG("Start of portForward requested!");
    }
    return RETURN_EXECUTE_OK;
}
void Core::createPortForward(string actionPF, string app, string port, string naMespace){
        PortForward* pf;
        CREATE_NEBEL("PortForward", pf, PortForward);
        
        pf->id = actionPF;
        pf->appName = app;
        pf->port = port;
        pf->nameSpace = naMespace;
        pf->component = this;

        portForwards[actionPF] = pf;
}
string Core::startAllPortforwards(){
    LOG("Core startPortforwards " << portForwardingRequested);
    portForwardingRequested = "";
    string commandList = "";
    for (std::map<string,PortForward*>::iterator child = portForwards.begin(); child != portForwards.end(); ++child){ 
        LOG("Starting portForward " << (child->first) );
        //startPortForward(child->second->appName, child->second->nameSpace, child->second->port);
        commandList += child->second->component->startPortForward( child->second );
        commandList += (std::next(child) != portForwards.end() ? ",\n" : "\n");
    }
    return "{\"commandList\":["+ commandList + "]}";
}
string Core::stopAllPortforwards(){
    LOG("Core stopPortforwards " << portForwardingRequested);
    portForwardingRequested = "";
    string commandList = "";
    for (std::map<string,PortForward*>::iterator child = portForwards.begin(); child != portForwards.end(); ++child){ 
        LOG("Stopinging portForward " << (child->first) );
        //startPortForward(child->second->appName, child->second->nameSpace, child->second->port);
        commandList += child->second->component->stopPortForward( child->second );
        commandList += (std::next(child) != portForwards.end() ? ",\n" : "\n");
    }
    return commandList;
}

string Core::doQuit(){
    string result = (string) "doQuit " + attributes[ATT_NAME]+"\n";
    stopAllPortforwards();
    for (std::map<string,PortForward*>::iterator child = portForwards.begin(); child != portForwards.end(); ++child){ 
        LOG("Deleting portForward " << (child->first) );
        DELETE_NEBEL("PortForward", child->second);
    }
    portForwards.clear();
    result += Component::doQuit(); // To delete childs
    return result;
}

Core::~Core(){
    doQuit();
    Core::coreInstance = nullptr;
}