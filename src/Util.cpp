
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
#include <vector>

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
void Util::writeFile(string file, string content){
    ofstream aFile;
    aFile.open( file );
    aFile << content;
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
string Util::loadFileRaw(string file){
    ifstream scriptFile;
    string script;
    scriptFile.open( file );

    if (scriptFile.is_open()){
        string line;
        while ( getline (scriptFile,line) ){
            script += line;
        }
        scriptFile.close();
    }
    return script;
}
// std::vector<unsigned char> Util::loadFileImage(string file){
//     std::ifstream input( file, std::ios::binary );
//     // copies all data into buffer
//     std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
//     input.close();
//     return buffer;
// }
string Util::loadFileImage(string file){
    std::ifstream in(file, std::ios::in | std::ios::binary);
    // if(in){
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        // res.set_content(contents.str(), "image/png");
    return contents.str();
}

// include <fstream>
// #include <iterator>
// #include <algorithm>
// int main()
// {
//     std::ifstream input( "C:\\Final.gif", std::ios::binary );
//     std::ofstream output( "C:\\myfile.gif", std::ios::binary );

//     std::copy( 
//         std::istreambuf_iterator<char>(input), 
//         std::istreambuf_iterator<char>( ),
//         std::ostreambuf_iterator<char>(output));
// }