
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <thread>

#include <iostream>
#include <fstream>
#include <vector>
#include <dlfcn.h> // dlopen, RTLD_LAZY, dlsym
// #include <csignal>

// #include "clj/inc/lang.CljString.hpp"
#include "inc/Lexical.hpp"
#include "inc/Control.hpp"
#include "inc/Core.hpp"
#include "inc/Config.hpp"

#define SERVER_REINTENTS 3

using namespace std;
namespace net = boost::asio;            // from <boost/asio.hpp>

Control control;

bool runServer = true;
Core* core = nullptr;

int main(){
    // CljString deleteLogs = "rm *.log";

    // cout << "Command " << deleteLogs;

    Component::systemCommand( "rm *.log" );
    Component::systemCommand( "rm *.out" );

    Lexical* lex = new Lexical();

    Config config( lex, nullptr );
    config.loadConfig( CONFIG_FILE );

    std::cout << "Hello World! v.a0.1.Tooling\n";
    LOG_INIT( "Hello World! Middleware v.a0.1.Tooling\n" );

    if (config.cfg( ATT_FRONT ) == CFG_ON ){
        string command = "lite-server -c Config.lite-server.json & ";
        Component::systemCommand(command); //, string filenaMeOut, string filenaMeErr);
    }
    
    core = new Core(lex, &config);
    string script = "./scripts/middlenebel.nebel";

    // core->setConfig(&config);

    LOG( "Core inited!" );
    core->load( script );

    int serverPort = stoi( core->config->cfg( ATT_SERVERPORT, CFG_DEFAULT_PORT) );
    runServer = config.cfg( ATT_DISABLE_SERVER ) != CFG_TRUE;
    if (runServer){
        LOG( "Starting server in port " << serverPort );
        std::cout << "Starting server in port " << serverPort;
    }else
        LOG("Main: Server is disabled by configuration! Skipping...");

    int retries = SERVER_REINTENTS;
    while (runServer && (retries-- >0)){
        bool reload = false;
        try{
            boost::asio::io_context io_context;
            tcp::acceptor acceptor(io_context, {tcp::v4(), serverPort});

            tcp::socket socket(io_context);

            try{
                acceptor.accept(socket);

                retries = SERVER_REINTENTS;
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
                if (readed){
                   reload = core->handleRequest(request, socket);
                }
            }catch(const std::exception& ex){
                LOG("Exception "<<ex.what());
                LOG("Exception  Handling the request!");
            }

            // Close the socket
            try{
                socket.shutdown(tcp::socket::shutdown_send);
            }catch(const std::exception& ex){
                LOG("Exception "<<ex.what());
                LOG(  "Exception shutdown socket!!!" );
            }
        }catch(const std::exception& ex){
            LOG("Exception "<<ex.what());
            LOG("Exception starting server! reloading...");
            std::this_thread::sleep_for(1000ms);
        }
        if (reload){
            if (core->newFileName != "")
                script = core->newFileName;
            LOG(  "Main reloading for " << script);
            LOG(  "Cleaning memory... objects: " + to_string( core->getObjectNum() ) );
            core->doQuit();
            LOG(  "Clean result objects: " + to_string( core->getObjectNum() ) );

            delete(core);
            delete(lex);

            lex = new Lexical();
            core = new Core(lex, &config);
            // core->setConfig(&config);
            LOG( "Core inited!" );
            core->load( script );
        }
        if (!core->portForwardingRequested.empty()){
            core->startAllPortforwards();
        }
        runServer = !core->quit;
    }
    // while ( runServer ){
    //     // long long used = control.getUsedMemory();
    //     LOG( "Starting server..." );
        
    //     Lexical* lex = new Lexical();
    //     core = new Core(lex, SERVER_PORT);

    //     LOG( "Core inited!" );
    //     core->load( script );

    //     runServer = core->runServer();

    //     if (core->newFileName != "")
    //         script = core->newFileName;

    //     LOG( "Main.doQuit!" );

    //     LOG(  "Cleaning memory... objects: " + to_string( core->getObjectNum() ) );
    //     core->doQuit();
    //     LOG(  "Clean result objects: " + to_string( core->getObjectNum() ) );

    //     delete(core);
    //     delete(lex);
    // }
    LOG( "Main.doQuit!" );

    LOG(  "Cleaning memory... objects: " + to_string( core->getObjectNum() ) );
    core->doQuit();
    config.doQuit();
    LOG(  "Clean result objects: " + to_string( core->getObjectNum() ) );

    delete(core);
    delete(lex);
    LOG( "Finished Middlenebel back-end!");
    std::cout << "Finished Middlenebel back-end!";
    return 0;
}

