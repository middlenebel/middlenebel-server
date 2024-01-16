
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>

#include "inc/Core.hpp"
#include "inc/Util.hpp"

#include "data/data.hpp"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace beast = boost::beast;         // from <boost/beast.hpp>

namespace fs = std::filesystem;

Core::Core(Lexical* lex, int serverPort){
    this->lex = lex;
    // core = this;
    parent = nullptr;
    quit = false;
    objectsNum = 0;
    this->serverPort = serverPort;
    init();
}

void Core::init(){
    // logContent = "";
    attributes["className"]="Core";
    attributes["name"]="Core";
    synLastChar='\0';
    synLastType=SYN_UNDEF;
    token="";
    newFileName = "";
    portForwardingRequested = "";

    // core->executors["test"] = this;
    executors["test"] = this;
}

void Core::load(string fileName){
    this->fileName = fileName;
    lex->load(fileName);

    // string sufix = ".nebel";
    // unsigned int pos = fileName.find(sufix);
    // LOG( "endsWith " << pos << " " << fileName.length() << " " << sufix.length() );

    if (Util::endsWith(fileName , ".nebel")){
        parse();
        if (portForwards.size() > 0){
            executors[ EXECUTOR_PORTFORWARD ] = this; 
            this->actions[ EXECUTOR_PORTFORWARD ] = EXECUTOR_PORTFORWARD;
        }
    }
    lex->close();
}

void Core::save(string fileName, string script){
    lex->save(fileName, script);
}

bool Core::handleRequest(http::request<http::string_body>& request, tcp::socket& socket) {
    bool reload = false;
    try{
        // Prepare the response message
        http::response<http::string_body> response;
        response.version(request.version());
        response.result(http::status::ok);
        response.set(http::field::server, "My HTTP Server");

        if(request.target() == "/status"){
            response.set(http::field::content_type, "text/plain");
            response.body() = "Core running ok!!! ";
        }else if(request.target() == "/components"){
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "[" + getJsonComponent() + "]";
            response.body() = json;
        }else if(request.target() == "/play"){
            string message = doPlay();
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"OK\" , \"message\" : \"Play success!\" }";
            response.body() = json;
        }else if(request.target() == "/destroy"){
            string message = doDestroy();
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"OK\" , \"message\" : \"Destroy success!\" }";
            response.body() = json;
            reload = true;
        }else if(request.target() == "/quit"){
            string message = "Quiting...!";
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"OK\" , \"message\" : \"Quit success!\" }";
            response.body() = json;
            quit = true;
        }else if(request.target() == "/reload"){//TODO
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"OK\" , \"message\" : \"Reload success!\" }";
            response.body() = json;
            reload = true;
        }else if(request.target() == "/save-script"){
            string script = request.body();
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"OK\" , \"message\" : \"Save success!\" }";
            response.body() = json;
            save( fileName, script );
            reload = true;
        }else if(request.target() == "/browserReload"){
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            // string json = "[{\"name\":\"Collection\",\"base\":\"/Collection\",\"children\": [{\"name\": \"script2.txt\",\"base\": \"/Collection\"}]}]";
            string json = "[" + getBrowserReload("") + "]";
            response.body() = json;
        }else if(request.target() == "/browserAction"){
            string action = request.body();
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = doBrowserAction(action);
            response.body() = json;

            if (this->newFileName != "")
                reload = true;
        }else if(request.target() == "/getLog"){
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            // string json = "[{\"name\":\"Collection\",\"base\":\"/Collection\",\"children\": [{\"name\": \"script2.txt\",\"base\": \"/Collection\"}]}]";
            
            string logContent = Util::loadFile( "middlenebel.log" );
            
            string json = 
                (string) "{ \"result\" : \"OK\" , \"message\" : \""+ logContent +"\" }";
            response.body() = json;
        }else if(request.target() == "/executeAction"){
            string action = request.body();
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = doExecuteAction( action );
            response.body() = json;

            if (this->newFileName != "")
                reload = true;
        }else{
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"KO\" , \"message\" : \"Server ERROR!\" }";
            response.body() = json;
        }
        response.prepare_payload();

        // Send the response to the client
        boost::beast::http::write(socket, response);
    }catch (...) {
        LOG( "Exception sending response!!!" );
    }
    return reload;
}

bool Core::runServer() {
    bool reload = false;

    LOG( "Server running!" );
    int retries = 9;
    while (!reload && (retries-- >0)){
        bool error = false;
        try{
            while ( !reload && !quit) {
                boost::asio::io_context io_context;
                tcp::acceptor acceptor(io_context, {tcp::v4(), serverPort});

                tcp::socket socket(io_context);

                try{
                    acceptor.accept(socket);
                }catch(const std::exception& ex){
                    LOG("Exception "<<ex.what());
                    LOG( "Exception accepting requests!!!" );
                }

                // Read the HTTP request
                boost::beast::flat_buffer buffer;
                http::request<http::string_body> request;
                
                bool readed = false;
                try{
                    boost::beast::http::read(socket, buffer, request);
                    readed = true;
                }catch(const std::exception& ex){
                    LOG("Exception "<<ex.what());
                    LOG( "Exception reading request!!!" );
                }

                // Handle the request
                try{
                    if (readed)
                        reload = handleRequest(request, socket);
                }catch(const std::exception& ex){
                    LOG("Exception "<<ex.what());
                    LOG("Exception  Handling the request!");
                    reload = false;
                }

                // Close the socket
                try{
                    socket.shutdown(tcp::socket::shutdown_send);
                }catch(const std::exception& ex){
                    LOG("Exception "<<ex.what());
                    LOG(  "Exception shutdown socket!!!" );
                }
            }
        }catch(const std::exception& ex){
            LOG("Exception "<<ex.what());
            LOG("Exception starting server! reloading...");
            error = true;
            std::this_thread::sleep_for(1000ms);
        }
        if (!reload && !reload && error && (retries>0) ){
            LOG("Waiting for restart...");
            std::this_thread::sleep_for(1000ms);
        }
    }
    LOG( (string) "Server end. Reload: " + (reload?"true":"false") );

    return reload;
}

string Core::getJsonComponent(){
    string components="{";
    components+=JSON_PROPERTY( "className" , attributes["className"])+",\n";
    components+=JSON_PROPERTY( "name" , attributes["name"] )+",\n";
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

string Core::getBrowserReload(string base){
    string json = "";
    string separator = "";
    for(const fs::directory_entry& entry : fs::directory_iterator("./scripts" + base)) {
        string name = entry.path().filename().string();

        json += separator;
        if ( is_regular_file( entry ) ){
            string baseFolder = (base=="") ? "/" : base;
            json += (string) "{" + 
                "\"name\":\"" + name +"\","+
                "\"base\":\"" + baseFolder +"\""+"}";

        } else if (is_directory( entry )) {

            std::string pathName = base + "/" + name; // Path to the directory

            string folderContent = getBrowserReload( pathName );
            if (folderContent=="")
                folderContent="{\"name\":\".\",\"base\":\""+ pathName+ "\"}";
                
            json += (string) "{" + 
                "\"name\":\"" + name +"\","+
                "\"base\":\"" + pathName +"\","+
                "\"children\":["+ folderContent +"]"+
            "}";

        }
        separator = ",";
    }
    return json;
}

string Core::doBrowserAction(string json){
    string result = "KO";
    string message = "Server ERROR!";

    Json::Value value;
    Json::Reader reader;
    bool isOk = reader.parse( json, value );
    if (isOk){
        const Json::Value actionValue = value["action"];
        string actionStr = actionValue.asString();

        const Json::Value baseValue = value["base"];
        string baseStr = baseValue.asString();

        const Json::Value newValue = value["newValue"];
        string newValueStr = newValue.asString();

        const Json::Value nameValue = value["value"];
        string nameStr = nameValue.asString();

        const Json::Value isFolderValue = value["isFolder"];
        bool isFolder = isFolderValue.asBool();

        string path = "./scripts"+baseStr;
        string pathTo = "./scripts"+baseStr;
        if (actionStr == "addFolder"){
            path += ((baseStr!="/")?"/":"") + newValueStr;
            fs::create_directory(path);
            result = "OK";
            message = (string)"Folder "+path+" created!";
        }else if (actionStr == "delete"){
            if (!isFolder){
                path += ((baseStr!="/")?"/":"") + nameStr;
            }
            fs::remove_all(path);
            result = "OK";
            message = (string)"Folder "+path+" deleted!";
        }else if (actionStr == "addFile"){
            if (!isFolder){
                path += ((baseStr!="/")?"/":"") + newValueStr;
            }
            std::fstream f(path, std::ios::out);
            f.close();
            result = "OK";
            message = (string)"Folder "+path+" deleted!";
        }else if (actionStr == "rename"){
            //if (!isFolder){
                path += ((baseStr!="/")?"/":"") + nameStr;
            //}
            pathTo += ((baseStr!="/")?"/":"") + newValueStr;
            LOG("Rename from "<<path<<" to "<<pathTo);
            try{
                fs::rename(path, pathTo);
                result = "OK";
                message = (string)"Folder "+path+" renamed to "+pathTo;
            }catch(const std::exception& ex){
                LOG("Exception "<<ex.what());
                result = "KO";
                message = (string)"Exception "+ex.what();
            }
        }else if (actionStr == "loadScript"){
            if (isFolder){
                message = (string)"A folder can't be loaded as script!";
            }else{
                path += ((baseStr!="/")?"/":"") + nameStr;
                this->newFileName = path;
            }
            result = "OK";
            message = (string)"Script "+path+" loaded!";
        }
    }

    return (string) "{ \"result\" : \""+ result +"\" , \"message\" : \""+ message +"\" }";
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
            //DEBUG 
            LOG(" Core calling executor "+executor->attributes["name"] );
            string response = RETURN_EXECUTE_KO;
            try{
                response = executor->execute( json );
            }catch(const std::exception& ex){
                LOG("Exception "<<ex.what());
                LOG("Exception in doExecuteAction! calling to " << executor->attributes["name"]);
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
        LOG("Start of portForward requested!");
    }
    return RETURN_EXECUTE_OK;
}

void Core::startPortforwards(){
    LOG("Core startPortforwards " << portForwardingRequested);
    portForwardingRequested = "";

    for (std::map<string,PortForward*>::iterator child = portForwards.begin(); child != portForwards.end(); ++child){ 
        LOG("Starting portForward " << (child->first) );
        //startPortForward(child->second->appName, child->second->nameSpace, child->second->port);
        child->second->component->startPortForward();
    }
}

string Core::doQuit(){
    string result = (string) "doQuit " + attributes["name"]+"\n";
    for (std::map<string,PortForward*>::iterator child = portForwards.begin(); child != portForwards.end(); ++child){ 
        LOG("Deleting portForward " << (child->first) );
        delete(child->second);
        objectsNum--;
    }
    Component::doQuit();
    return result;
}