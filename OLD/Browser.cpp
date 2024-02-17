
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>

#include "inc/Util.hpp"
#include "inc/Config.hpp"
#include "inc/Browser.hpp"

namespace fs = std::filesystem;

Browser::Browser(){
    synLastChar='\0';
    synLastType=SYN_UNDEF;
    newFileName = "";
}

void Browser::load(string fileName){
    this->fileName = fileName;
    lex->load(fileName);
}

void Browser::save(string fileName, string script){
    lex->save(fileName, script);
}

bool Browser::handleRequest(http::request<http::string_body>& request, tcp::socket& socket) {
    bool reload = false;
    try{
        // Prepare the response message
        http::response<http::string_body> response;
        response.version(request.version());
        response.result(http::status::ok);
        response.set(http::field::server, "My HTTP Server");

        if(request.target() == "/status"){
            response.set(http::field::content_type, "text/plain");
            response.body() = "Browser running ok!!! ";
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
            LOG("Destroying platform in target systems...");
            string message = doDestroy();
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"OK\" , \"message\" : \"Destroy success!\" }";
            response.body() = json;
            reload = true;
        }else if(request.target() == "/quit"){
            LOG("Close server requested!!!");
            string message = "Quiting...!";
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"OK\" , \"message\" : \"Quit success!\" }";
            response.body() = json;
            quit = true;
        }else if(request.target() == "/reload"){//TODO
            LOG("Reload requested!!!");
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            string json = "{ \"result\" : \"OK\" , \"message\" : \"Reload success!\" }";
            response.body() = json;
            reload = true;
        }else if(request.target() == "/save-script"){
            LOG("Save requested!!!");
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
        }else if(request.target() == "/clearLog"){
            response.set(http::field::content_type, "application/json");
            response.set(http::field::access_control_allow_credentials, "true" );
            response.set(http::field::access_control_allow_origin, "*" );
            Util::emptyFile(FILE_MIDDLENEBEL_LOG);
            string logContent = "Log cleaned!";
            
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
            // DEBUG("HTTP_REQUEST "<<request.target()); //index.html
            // DEBUG("Serving file... "<<request.target());
            // string url = request.target().to_string(); //string::npos
            // string contentType = "text/html; charset=utf-8";
            // if (url.find("//") == 0){
            //     url = url.substr(1);
            // }
            // if (url.find(".js") != string::npos){
            //     contentType = "text/javascript";
            // }
            // if (url.find(".css") != string::npos){
            //     contentType = "text/css";
            // }
            // DEBUG("Looking for "+url);
            // string data = Util::loadFile("./dist/front-0"+url);

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

string Browser::getBrowserReload(string base){
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

string Browser::doBrowserAction(string json){
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



