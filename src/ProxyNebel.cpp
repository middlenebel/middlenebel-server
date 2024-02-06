
#include <chrono>
#include <thread>

#include <iostream>
#include <fstream>
#include <vector>
#include <dlfcn.h> // dlopen, RTLD_LAZY, dlsym

// Wrappers
#include "inc/wrappers/HttpServer.hpp"
#include "inc/wrappers/HttpClient.hpp"

// #include "clj/inc/lang.CljString.hpp"
#include "inc/Lexical.hpp"
#include "inc/Control.hpp"
#include "inc/Core.hpp"
#include "inc/Config.hpp"

#define SERVER_REINTENTS 3

using namespace std;

int main(){
    bool runServer = true;
    HttpServer svr;

    Component::systemCommand( "rm *.log" );
    Component::systemCommand( "rm *.out" );

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

std::cout << "GET hi...\n"; 
    svr.Get("/hi", [scheme_host_port](const Request &, Response &res) {
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
std::cout << "GET components...\n"; 
    svr.Get("/components", [scheme_host_port](const Request &, Response &res) {
        cout << "PROXY/components Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/components")) { 
            res.status = resCli.value().status;
            res.body = resCli.value().body;
         }
        //res.set_content("Hello World!", "text/plain");
    });
    svr.Get("/play", [scheme_host_port](const Request &, Response &res) {
        cout << "PROXY/play Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/play")) { 
            res.status = resCli.value().status;
            res.body = resCli.value().body;
        }
        // res.set_content("Hello World!", "text/plain");
    });
    svr.Get("/destroy", [scheme_host_port](const Request &, Response &res) {
        cout << "PROXY/destroy Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/destroy")) { 
            res.status = resCli.value().status;
            res.body = resCli.value().body;
        }
        // res.set_content("Hello World!", "text/plain");
    });
    svr.Get("/reload", [scheme_host_port](const Request &, Response &res) {
        cout << "PROXY/reload Received" << "\n";
        HttpClient cli( scheme_host_port ); 
        if (HttpClient::Result resCli = cli.Get("/reload")) { 
            res.status = resCli.value().status;
            res.body = resCli.value().body;
        }
        // res.set_content("Hello World!", "text/plain");
    });
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
    // int retries = SERVER_REINTENTS;
    // while (runServer && (retries-- >0)){
    //     bool reload = false;
    //     try{
    //         boost::asio::io_context io_context;
    //         tcp::acceptor acceptor(io_context, {tcp::v4(), serverPort});

    //         tcp::socket socket(io_context);

    //         try{
    //             acceptor.accept(socket);

    //             retries = SERVER_REINTENTS;
    //         }catch(const std::exception& ex){
    //             LOG("Exception "<<ex.what());
    //             LOG( "Exception accepting requests!!!" );
    //         }

    //         // Read the HTTP request
    //         boost::beast::flat_buffer buffer;
    //         http::request<http::string_body> request;
                
    //         bool readed = false;
    //         try{
    //             boost::beast::http::read(socket, buffer, request);
    //             readed = true;
    //         }catch(const std::exception& ex){
    //             LOG("Exception "<<ex.what());
    //             LOG( "Exception reading request!!!" );
    //         }

    //         // Handle the request
    //         try{
    //             if (readed){
    //                reload = core->handleRequest(request, socket);
    //             }
    //         }catch(const std::exception& ex){
    //             LOG("Exception "<<ex.what());
    //             LOG("Exception  Handling the request!");
    //         }

    //         // Close the socket
    //         try{
    //             socket.shutdown(tcp::socket::shutdown_send);
    //         }catch(const std::exception& ex){
    //             LOG("Exception "<<ex.what());
    //             LOG(  "Exception shutdown socket!!!" );
    //         }
    //     }catch(const std::exception& ex){
    //         LOG("Exception "<<ex.what());
    //         LOG("Exception starting server! reloading...");
    //         std::this_thread::sleep_for(1000ms);
    //     }
    //     if (reload){
    //         if (core->newFileName != "")
    //             script = core->newFileName;
    //         LOG(  "Main reloading for " << script);
    //         LOG(  "Cleaning memory... objects: " + to_string( core->getObjectNum() ) );
    //         core->doQuit();
    //         LOG(  "Clean result objects: " + to_string( core->getObjectNum() ) );

    //         delete(core);
    //         delete(lex);

    //         lex = new Lexical();
    //         core = new Core(lex, &config);
    //         // core->setConfig(&config);
    //         LOG( "Core inited!" );
    //         core->load( script );
    //     }
    //     if (!core->portForwardingRequested.empty()){
    //         core->startAllPortforwards();
    //     }
    //     runServer = !core->quit;
    // }
    
    LOG( "ProxyNebel.doQuit!" );

    LOG(  "Cleaning memory... objects: " + to_string( config.getObjectNum() ) );
    config.doQuit();
    LOG(  "Clean result objects: " + to_string( config.getObjectNum() ) );

    LOG( "Finished Middlenebel back-end!");
    std::cout << "Finished Middlenebel back-end!";
    return 0;
}

