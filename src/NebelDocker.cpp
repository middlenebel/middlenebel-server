
#include <chrono>
#include <thread>

#include <iostream>
#include <fstream>
#include <vector>
#include <dlfcn.h> // dlopen, RTLD_LAZY, dlsym

#include <future>
#include <thread>
#include <utility>

// Wrappers
#include "inc/wrappers/HttpServer.hpp"

// #include "clj/inc/lang.CljString.hpp"
#include "inc/Lexical.hpp"
#include "inc/Control.hpp"
#include "inc/Core.hpp"
#include "inc/Config.hpp"

#define SERVER_REINTENTS 3

using namespace std;

HttpServer svr;
// @See: https://www.modernescpp.com/index.php/promise-and-future/
void server_func(std::promise<int>&& reloadPromise ){
    std::cout << "Starting thread for server...\n"; 
    // HttpServer svr;
    Core* core = Core::getInstance();
    core->setReloadPromise( std::move(reloadPromise) );
    std::string strServerPort = core->config->cfg(ATT_NEBELPORT, CFG_NEBELPORT);
    int nebelPort = stoi( strServerPort );
    std::cout << "Starting server at port " << nebelPort << "\n"; 

    svr.listen("0.0.0.0", nebelPort);
    std::cout << "Stopping server at port " << nebelPort << "\n";
}

int main(){
    bool runServer = true;

    Component::systemCommand( "rm *.log" );
    Component::systemCommand( "rm *.out" );

    Core* core = Core::getInstance();
    Config config( core );
    config.loadConfig( CONFIG_FILE );

    core->setConfig(&config);
  
    //string nebelVersion = config.cfg( VERSION, "Middlenebel v0.1.3-alpha Nebel-Docker" );
    std::cout << "Hello World!\n";
    LOG_INIT( "Hello World!\n" );

    string script = "./scripts/middlenebel.nebel";

    //core->load( script );
    LOG( "Core ready!" );
    std::cout << "Core ready!\n";

    svr.Get("/hi", [](const Request &, Response &res) {
        cout << "hi OK\n";
        res.set_content("Hello World! Hi!", "text/plain");
    })
    .Get("/status", [](const Request &req, Response &res) {
        Core::getInstance()->getStatus(req, res);
        //res.set_content("Up!", "text/plain");
    })
    .Get("/components", [](const Request &req, Response &res) {
        Core::getInstance()->getComponents(req, res);
    })
    .Get("/play", [](const Request &req, Response &res) {    Core::getInstance()->getPlay(req, res); })
    .Get("/destroy", [](const Request &req, Response &res) { Core::getInstance()->getDestroy(req, res); })
    .Get("/reload", [](const Request &req, Response &res) {  Core::getInstance()->getReload(req, res); })
    .Post("/save-script", [](const Request &req, Response &res) {       Core::getInstance()->postSaveScript(req, res); })
    .Post("/browserReload", [](const Request &req, Response &res) {     Core::getInstance()->postBrowserReload(req, res); })
    .Post("/browserReload", [](const Request &req, Response &res) {     Core::getInstance()->postBrowserAction(req, res); })
    .Get("/getLog", [](const Request &req, Response &res) {      Core::getInstance()->getLog(req, res); })
    .Get("/clearLog", [](const Request &req, Response &res) {    Core::getInstance()->getClearLog(req, res); })
    .Post("/executeAction", [](const Request &req, Response &res) { Core::getInstance()->getExecuteAction(req, res); });
    
    while (runServer){
        std::promise<int> reloadPromise;
        std::future<int> reloadResult= reloadPromise.get_future();
        std::cout << "Launching thread for server...\n";
        std::thread serverThread(server_func, std::move(reloadPromise));
        std::cout << "Waiting promise for reload server...\n";
        int reload = reloadResult.get();

        std::cout << "Future receive with: " << reload << "\n";
        
        if (reload != 0){
            cout << "Reload requested!!!";
        }
        // reloadResult= reloadPromise.get_future();
        svr.stop();

        serverThread.join();
    }
    LOG( "Main.doQuit!" );

    LOG(  "Cleaning memory... objects: " + to_string( core->getObjectNum() ) );
    core->doQuit();
    config.doQuit();
    LOG(  "Clean result objects: " + to_string( core->getObjectNum() ) );

    delete(core);
    // delete(lex);
    LOG( "Finished Middlenebel back-end!");
    std::cout << "Finished Middlenebel back-end!";
    return 0;
}

