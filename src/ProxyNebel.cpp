
#include <chrono>
#include <thread>

#include <iostream>
#include <fstream>
#include <vector>
#include <dlfcn.h> // dlopen, RTLD_LAZY, dlsym

// Wrappers
#include "inc/wrappers/HttpServer.hpp"
#include "inc/wrappers/HttpClient.hpp"
#include <jsoncpp/json/json.h>

// #include "clj/inc/lang.CljString.hpp"
#include "inc/Lexical.hpp"
#include "inc/Control.hpp"
#include "inc/Core.hpp"
#include "inc/Config.hpp"

#define SERVER_REINTENTS 3

using namespace std;
string executeCommandList( string commandList, string scheme);
void processRequest(string url, Response& res);

int main(){
    bool runServer = true;
    HttpServer svr;

    // svr.Options(R"(\*)", [](const auto& req, auto& res) {
    //     res.set_header("Allow", "GET, POST, HEAD, OPTIONS");
    // });

    // svr.Options("/components", [](const auto& req, auto& res) {
    //     res.set_header("Access-Control-Allow-Origin", "*" ); //req.get_header_value("Origin").c_str());
    //     res.set_header("Access_Control_Allow_Credentials", "true" );
    //     // res.set_header("Allow", "GET, POST, HEAD, OPTIONS");
    //     // res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Origin, Authorization");
    //     // res.set_header("Access-Control-Allow-Methods", "OPTIONS, GET, POST, HEAD");
    // });

    // Component::systemCommand( "rm *.log" );
    // Component::systemCommand( "rm *.out" );

    Config config( nullptr );
    config.loadConfig( CONFIG_FILE );

    //string nebelVersion = config.cfg( VERSION, "Middlenebel v0.1.3-alpha Nebel-Docker" );
    std::cout << "Hello World!\n";
    LOG_INIT( "Hello World!\n" );

    if (config.cfg( ATT_FRONT ) == CFG_LITE ){  //TODO liteServer has config file
        string command = "lite-server -c Config.lite-server.json & ";
        Component::systemCommand(command); //, string filenaMeOut, string filenaMeErr);
    }else if (config.cfg( ATT_FRONT ) == CFG_DOCKER ){
        string frontName = config.cfg(ATT_FRONTNAME, CFG_FRONTNAME);
        string strWebPort = config.cfg( ATT_WEBPORT, CFG_WEBPORT);
        
        std::cout << "Starting docker Nebel-web in port " << strWebPort;
        string command = "docker run -p "+strWebPort+":80 "+frontName+" & ";
        Component::systemCommand(command, "front-start.out", "front-error.out");
    }

std::cout << "nebelPort...\n";    
    // FRONT---web--------> WEBPORT [WebAngular ]---( liteServer / docker )
    // FRONT---comands----> PROXYPORT [Proxy ] ----> NEBELPORT [NEBELDOCKER]

    string scheme_host_port = "http://localhost:" + config.cfg(ATT_NEBELPORT, CFG_NEBELPORT);
    // HttpClient cli( nebelPort );

    string script = "./scripts/middlenebel.nebel";

    svr.Get("/hi", [scheme_host_port](const Request &, Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*" );
        res.set_header("Access_Control_Allow_Credentials", "true" );
        cout << "PROXY/hi Received" << "\n";
        HttpClient cli( scheme_host_port );
        if (HttpClient::Result resCli = cli.Get("/hi")) {
        //   if (resCli->status == httplib::StatusCode::OK_200) {
        //     //std::cout << res.value().body << std::endl;
        //     res.body = resCli.value().body;
        //   }
          res.status = resCli.value().status;
          res.body = resCli.value().body;
        }
        //res.set_content("Hello World!kkkk", "text/plain");
    });

    svr.Get("/components", [scheme_host_port](const Request &, Response &res){
        res.set_header("Access-Control-Allow-Origin", "*" );
        res.set_header("Access_Control_Allow_Credentials", "true" );
        cout << "PROXY/components Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/components")) { 
            res.set_content(resCli.value().body, "application/json");
         }
    });
    svr.Get("/play", [scheme_host_port](const Request &, Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*" );
        res.set_header("Access_Control_Allow_Credentials", "true" );        
        cout << "PROXY/play Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/play")) { 
            res.status = resCli.value().status;
            res.body = executeCommandList( resCli.value().body, scheme_host_port );
        }
        // res.set_content("Hello World!", "text/plain");
    });
    svr.Get("/destroy", [scheme_host_port](const Request &, Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*" );
        res.set_header("Access_Control_Allow_Credentials", "true" );        
        cout << "PROXY/destroy Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/destroy")) { 
            //res.status = resCli.value().status;
            res.set_content(resCli.value().body, "application/json");
        }else{
            res.status=505; //TODO search errors
        }
    });
    svr.Post("/executeAction", [scheme_host_port](const Request &req, Response &res) { 
        res.set_header("Access-Control-Allow-Origin", "*" );
        res.set_header("Access_Control_Allow_Credentials", "true" );        
        HttpClient cli( scheme_host_port ); 
        const string params = req.body;
        if (HttpClient::Result resCli = cli.Post("/executeAction", params)) { 
            //DEBUG cout << "PROXY Execute sent " << params << "\n";
            //DEBUG cout << "PROXY Execute received " << res.body << "\n";

            executeCommandList( resCli.value().body , scheme_host_port );
            res.set_content(RETURN_EXECUTE_OK, "application/json");
        }
    });
  
    svr.Get("/reload", [scheme_host_port](const Request &, Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*" );
        res.set_header("Access_Control_Allow_Credentials", "true" );        
        cout << "PROXY/reload Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/reload")) { 
            res.status = resCli.value().status;
            res.body = resCli.value().body;
        }
        // res.set_content("Hello World!", "text/plain");
    });

    svr.Get("/getLog", [scheme_host_port](const Request &, Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*" );
        res.set_header("Access_Control_Allow_Credentials", "true" );        
        cout << "PROXY/getLog Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/getLog")) { 
            res.status = resCli.value().status;
            res.body = resCli.value().body;
        }
        // res.set_content("Hello World!", "text/plain");
    });

    if (config.cfg(ATT_FRONT) == CFG_INTERNAL){
        svr.Get("/assets/:url", [&](const Request& req, Response& res) { 
            auto url = req.path_params.at("url");
            processRequest( "/assets/"+url, res);
        });
        svr.Get("/:url", [&](const Request& req, Response& res) { 
            //svr.Get(R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)", [&](const Request& req, Response& res) { 
            auto url = req.path_params.at("url");
            processRequest( url, res);
        });
    }

std::cout << "GETs.\n"; 
    string strProxyPort = config.cfg( ATT_PROXYPORT, CFG_PROXYPORT);

    short unsigned int proxyPort = stoi( strProxyPort );
    runServer = config.cfg( ATT_DISABLE_PROXY ) != CFG_TRUE;
std::cout << "runServer " << runServer << ".\n";    
    if (runServer){
        LOG( "Starting proxy in port " << proxyPort );
        std::cout << "Starting proxy in port " << proxyPort << "\n";
    }else
        LOG("Main: Server is disabled by configuration! Skipping...");

    HttpClient cli( scheme_host_port ); 
    std::cout << "Looking for Nebel server at port " << scheme_host_port << "\n";
    if (HttpClient::Result resCli = cli.Get("/hi")) { 
        std::cout << "Nebel server conected in port " << scheme_host_port << "\n";
    }
std::cout << "proxyPort...\n";
    svr.listen("0.0.0.0", proxyPort);
std::cout << "proxyPort!\n";
    
    LOG( "ProxyNebel.doQuit!" );

    LOG(  "Cleaning memory... objects: " + to_string( config.getObjectNum() ) );
    config.doQuit();
    LOG(  "Clean result objects: " + to_string( config.getObjectNum() ) );

    LOG( "Finished Middlenebel back-end!");
    std::cout << "Finished Middlenebel back-end!";
    return 0;
}

void processRequest(string url, Response& res){
    if (url == "") url = "index.html";
        cout << "DEBUG: URL " << url << "\n";
        string content = "";
        if (Util::endsWith( url, ".png")){
            // std::vector<unsigned char> binaryData = Util::loadFileImage("./dist/" + url);
            // content = reinterpret_cast<char*> (&binaryData[0]);
            content = Util::loadFileImage("./dist/" + url);  
        }else
            content = Util::loadFileRaw( "./dist/" + url );

        //cout << "DEBUG: Content " << content << "\n";
        string contentType = "text/plain";
        if (Util::endsWith( url, ".html")) contentType="text/html";
        else if (Util::endsWith( url, ".css")) contentType="text/css";
        else if (Util::endsWith( url, ".ico")) contentType="image/x-icon";
        else if (Util::endsWith( url, ".js")) contentType="application/javascript";
        else if (Util::endsWith( url, ".png")) contentType="image/png";
        else if (Util::endsWith( url, ".svg")) contentType="image/svg+xml";

        //string strContent = reinterpret_cast<char*> (&binaryData[0]);

        res.set_content( content, contentType);
}

// server.Get("/test", [](const http::Request&, http::Response& res){
//     std::ifstream in(filename, std::ios::in | std::ios::binary);
//     if(in){
//         std::ostringstream contents;
//         contents << in.rdbuf();
//         in.close();
//         res.set_content(contents.str(), "image/png");
//     }else{
//         res.status = 404;
//     }
// });

string executeCommandList( string commandList, string scheme_host_port){
    cout << "executeCommandList\n";
    //DEBUG cout << commandList << "\n";
    string response="";
    Json::Value root;
    Json::Reader reader; //std::stringstream sstr(json);
    bool isOk = reader.parse( commandList, root ); // sstr >> api;
    if (isOk){
        Json::Value cmdList = root["commandList"];
        cout << "executeCommandList JSON Ok " << cmdList.size() << " commands\n";
        for ( unsigned int index = 0; index < cmdList.size(); ++index ){
            cout << "executeCommandList Item " << index << "\n";
            const Json::Value cmdValue = cmdList[index]["command"];
            string cmdValueStr = cmdValue.asString();

            cout << "executeCommandList CMD " << cmdValueStr << "\n";

            if (cmdValueStr == "ERROR") break;

            const Json::Value msgValue = cmdList[index]["msg"];
            string msgValueStr = msgValue.asString();

            const Json::Value fileValue = cmdList[index]["fileName"];
            if (fileValue){
                string fileValueStr = msgValue.asString();

                const Json::Value contValue = cmdList[index]["fileContent"];
                string contValueStr = msgValue.asString();

                // if ( contValueStr != ""){
                //     Util::writeFile( fileValueStr, contValueStr);
                // }
                HttpClient cli( scheme_host_port ); 
                if (HttpClient::Result resCli = cli.Get("/"+fileValueStr)) { 
                    // res.status = resCli.value().status;
                    // res.body = resCli.value().body;
                    Util::writeFile( fileValueStr, resCli.value().body);
                    cout << "DEBUG Writted " << fileValueStr << "\n";
                };
            }
            
            Component::systemCommand( cmdValueStr , "system.out", "system-error.out");
            response+=msgValueStr+"\n";
        }
    }else{
        response+="ERROR processing request!\n";
        cout << "executeCommandList: ERROR in JSON\n";
        cout << reader.getFormattedErrorMessages() << "\n";
    }
    return response;
}
