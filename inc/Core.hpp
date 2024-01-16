
#if !defined( CORE_H )
#define CORE_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include "fields_alloc.hpp"

#include "Component.hpp"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace beast = boost::beast;         // from <boost/beast.hpp>

#define SERVER_PORT 8080

class Core : public Component{
    private:
        void init();
        bool handleRequest(http::request<http::string_body>& request, tcp::socket& socket);
        void send_file(beast::string_view target, tcp::socket& socket_);
        void send_bad_response(  http::status status, std::string const& error, tcp::socket& socket_);
        string doExecuteAction(string json);
        void destroyProblems();
        
        bool quit;

        tcp::socket* socketRef;
        short unsigned int serverPort;
    public:
        void abortServer();
        // string logContent;
        string fileName;
        string newFileName;

        Core(Lexical*, int serverPort = SERVER_PORT);
        bool runServer();
        
        string getJsonComponent();
        void load(string file);
        void save(string file, string script);
        // void log(string line);
        // TODO friend ostream& operator<<(ostream& os, const string& line);

        string getBrowserReload(string base);
        string doBrowserAction(string action);
};

#endif // !defined( CORE_H )