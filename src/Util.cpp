
/* NOBOOST
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
*/
#include <cstdlib>
#include <string>
#include <ctime>

#include "inc/utils/Util.hpp"
#include "inc/Core.hpp"

/* NOBOOST
namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
*/
using namespace std;

ofstream Util::middlenebelLog;
ofstream Util::middlenebelLogFile;

std::time_t Util::now(){
    return std::time(0);
}

bool Util::endsWith(string data, string sufix){
    unsigned int pos = data.find(sufix);
    return (pos == (data.length() - sufix.length()));
}
void Util::emptyFile(string file){
    ofstream aFile;
    aFile.open( file );
    aFile.close();
}
string Util::loadFile(string file){
    ifstream scriptFile;
    string script;
    scriptFile.open( file );

    if (scriptFile.is_open()){
        string line;
        while ( getline (scriptFile,line) ){
            script += line + "\\n";
        }
        scriptFile.close();
    }
    // std::replace( script.begin(), script.end(), '"', "'");
    //DEBUG cout << "File: " << script << "\n";
    return script;
}
/* NOBOOST
//https://stackoverflow.com/questions/59814196/boost-asio-connect-connection-refused
const string Util::getHttp(string theHost, string thePort, string theTarget){
    string config="EXIT_SUCCESS";
    try{
        auto const host = theHost;
        auto const port = thePort;
        auto const target = theTarget;
        int version = 11; //10; //: 11;

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        stream.connect(results);

        // Set up an HTTP GET request message
        http::request<http::string_body> req{http::verb::get, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the HTTP request to the remote host
        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        // Write the message to standard out
        //DEBUG std::cout << res << std::endl;
        //https://github.com/boostorg/beast/issues/819
        //s = boost::asio::buffer_cast<const char*>(res.body.data());

        config = boost::beast::buffers_to_string(res.body().data());

        // Gracefully close the socket
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // not_connected happens sometimes
        // so don't bother reporting it.
        //
        if(ec && ec != beast::errc::not_connected)
            throw beast::system_error{ec};

        // If we get here then the connection is closed gracefully
    }
    catch(std::exception const& e)
    {
        //DEBUG std::cerr << "Error: " << e.what() << std::endl;
        config = (string) MIDDLENEBEL_EXIT_FAILURE; //EXIT_FAILURE int 1
    }
     //return config; //EXIT_SUCCESS int 0 (stdlib)

//https://github.com/boostorg/beast/issues/819
//      You can use std::string s = boost::asio::buffer_cast<const char*>(res.body.data());

// If the body is of something like multi_buffer, the you need to iterate through the buffer sequence and append it to the string, something like:

// for (auto seq : buf_sequence) {
//   auto* cbuf = boost::asio::buffer_cast<const char*>(seq);
//   s.append(cbuf, boost::asio::buffer_size(seq));
// } 
     return config;
}
*/