
#include <iostream>
#include <fstream>
#include <vector>
#include <dlfcn.h> // dlopen, RTLD_LAZY, dlsym
#include <csignal>

#include <boost/asio.hpp>

#include "inc/Lexical.hpp"
#include "inc/Control.hpp"
#include "inc/Core.hpp"

// void* loadPlugin(char* librarypath);
// void testPlugin(void *libhandle);

using namespace std;
namespace net = boost::asio;            // from <boost/asio.hpp>

Control control;

// // "Loop" forever accepting new connections.
// void http_server(tcp::acceptor& acceptor, tcp::socket& socket, Core* core)
// {
//   acceptor.async_accept(socket,
//       [&](beast::error_code ec)
//       {
//           if(!ec)
//               //std::make_shared<Core>(std::move(socket))->start();
//               std::move(core)->start();
//           http_server(acceptor, socket, std::move(core));
//       });
// }

bool runServer = true;
Core* core = nullptr;

// void signalHandler( int signum ) {
//     cout << "SIGINT!!! " << signum << "\n";
//     LOG("SIGINT received!");
//     runServer = false; 
//     if (core != nullptr){
//         core->abortServer();
//         LOG("Quiting Core!");
//     }
// }

int main(){
    cout << "Hello World! v.a0.1.Tooling\n";

    LOG_INIT( "Starting Middlenebel back-end...");
    // LOG_INIT_FILE( "Starting Middlenebel back-end...", "memory.log");
 
    string script = "./scripts/middlenebel.nebel";

    // signal(SIGINT, signalHandler);

    while ( runServer ){
        // long long used = control.getUsedMemory();
        LOG( "Starting server..." );
        
        Lexical* lex = new Lexical();
        core = new Core(lex, SERVER_PORT);

        LOG( "Core inited!" );
        core->load( script );

        runServer = core->runServer();

        if (core->newFileName != "")
            script = core->newFileName;

        LOG( "Main.doQuit!" );

        LOG(  "Objects num: " + to_string( core->getObjectNum() ) );
        core->doQuit();
        LOG(  "Objects num: " + to_string( core->getObjectNum() ) );

        delete(core);
        delete(lex);
    }

    LOG( "Finished Middlenebel back-end!");
    return 0;
}

