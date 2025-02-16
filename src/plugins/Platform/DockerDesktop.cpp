
// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
// #include <boost/beast/version.hpp>
// #include <boost/asio/connect.hpp>
// #include <boost/asio/ip/tcp.hpp>

// #include <boost/asio.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

#include "../../inc/Core.hpp"
#include "../../inc/Component.hpp"
#include "../../inc/utils/Util.hpp"
#include "../../incPlugins/DockerDesktop.hpp"

// namespace beast = boost::beast;     // from <boost/beast.hpp>
// namespace http = beast::http;       // from <boost/beast/http.hpp>
// namespace net = boost::asio;        // from <boost/asio.hpp>
// using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

DockerDesktop::DockerDesktop(Component* parent):Component(parent){
     init();
     parent->addComponent(this);
}

void DockerDesktop::init(){
     attributes[ATT_CLASSNAME]="DockerDesktop";
     attributes[ATT_NAME]="DockerDesktop";
     // getConfig();
}

void DockerDesktop::parse(){
    LOG( "Parse by DockerDesktop..." );
    
    while ( readToken() ){
        //DEBUG LOG( "\nDockerDesktop TOKEN: " << token << "\n";

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        else return;
    }
}
