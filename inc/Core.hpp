
#if !defined( CORE_H )
#define CORE_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include "fields_alloc.hpp"

#include "Component.hpp"

#define EXECUTOR_PORTFORWARD "PortForwardAdmin"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace beast = boost::beast;         // from <boost/beast.hpp>

#define SERVER_PORT 8080

using namespace std;

struct PortForward{
    string id;
    string appName;
    string port;
    string nameSpace;
    Component* component;
};

class Core : public Component{
    private:
        void init();
        void send_file(beast::string_view target, tcp::socket& socket_);
        void send_bad_response(  http::status status, std::string const& error, tcp::socket& socket_);
        string doExecuteAction(string json);
        void destroyProblems();
        
        bool quit;

        short unsigned int serverPort;
    public:
        // string logContent;
        string fileName;
        string newFileName;
        string portForwardingRequested;

        std::map<string, PortForward*> portForwards;

        Core(Lexical*, int serverPort = SERVER_PORT);
        bool runServer();
        bool handleRequest(http::request<http::string_body>& request, tcp::socket& socket);
        
        // addPortFordward(string id, PortForward pf){ portForwards[id] = }
        string getJsonComponent();
        string getJsonPortForwards();

        void load(string file);
        void save(string file, string script);
        // void log(string line);
        // TODO friend ostream& operator<<(ostream& os, const string& line);

        string getBrowserReload(string base);
        string doBrowserAction(string action);
        string execute( string json);
        void startPortforwards();

        string doQuit();
};

#endif // !defined( CORE_H )