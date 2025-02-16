/*
 * Copyright (c) 2024, David Hernández Tejada
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#if !defined( CORE_H )
#define CORE_H

#include <iostream>
#include <future>
#include <optional>

#include "PortForward.hpp"
#include "Component.hpp"
#include "Config.hpp"
#include "../incPlugins/Platform.hpp"

// Wrappers
#include "wrappers/HttpServer.hpp"

#define EXECUTOR_PORTFORWARD "PortForwardAdmin"
#define FILE_MIDDLENEBEL_LOG "middlenebel.log"

using namespace std;

class Core : public Component{
    private:
        static Core* coreInstance;
        std::list<string> logStrings;

        Core();

        void init();
        Component* loadPlugin4Core(string pluginName);
        /* NOBOOST
        void send_file(beast::string_view target, tcp::socket& socket_);
        void send_bad_response(  http::status status, std::string const& error, tcp::socket& socket_);
        */
        string doExecuteAction(string json);
        void destroyProblems(); //TODO (?)

        short unsigned int serverPort;

        std::promise<int> reloadPromise;
        
    public:
        Config* config;
        // string logContent;
        std::optional<string> oFileName;
        string newFileName;
        string portForwardingRequested;
        bool quit;

        std::map<string, PortForward*> portForwards;
        //DEPRECATED std::list<string> commandList;

        static Core* getInstance();
        void log(string line);
        string getLog();

        Core(Core*) = delete;
        void operator=(const Core &) = delete;
        ~Core();
        virtual string doQuit();
        
        void setConfig(Config* config);
        
        void setReloadPromise( std::promise<int>&& reloadPromise );

        // addPortFordward(string id, PortForward pf){ portForwards[id] = }
        string getJsonComponent();
        string getJsonPortForwards();

        void load(string file);
        void save(string file, string script);
        // void log(string line);
        // TODO friend ostream& operator<<(ostream& os, const string& line);

        string doBrowserAction(string action);
        string execute( string json);

        void createPortForward(string actionPF, string app, string port, string naMespace);
        string startAllPortforwards();
        string stopAllPortforwards();

        bool getStatus(const Request &req, Response &res); 
        bool getComponents(const Request &req, Response &res);
        bool getPlay(const Request &req, Response &res);
        bool getDestroy(const Request &req, Response &res);
        bool getQuit(const Request &req, Response &res);
        bool getReload(const Request &req, Response &res);
        bool postSaveScript(const Request &req, Response &res);
        bool getLog(const Request &req, Response &res);
        bool getClearLog(const Request &req, Response &res);
        bool getExecuteAction(const Request &req, Response &res);
};

#endif // !defined( CORE_H )