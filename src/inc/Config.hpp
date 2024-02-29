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

#if !defined( CONFIG_H )
#define CONFIG_H

#include "../inc/Component.hpp"
#include "../inc/Lexical.hpp"

#define VERSION "Middlenebel v0.1.3-alpha Nebel-Docker"
// -------------------------------------------------------
#define CONFIG_FILE_PROXY "./ConfigProxy.nebel"
#define CONFIG_FILE_NEBEL "./ConfigNebel.nebel"

// FRONT---web--------> WEBPORT [WebAngular ]---( liteServer / docker )
// FRONT---comands----> PROXYPORT [Proxy ] ----> NEBELPORT [NEBELDOCKER]
// ---------------------------------- ATTRIBUTES ---------
#define ATT_DISABLE_PARSE           "disableParse"
#define ATT_DISABLE_PROXY           "disableProxy"
#define ATT_USE_CONFIG              "useConfig"
#define ATT_DUMP_CONFIG             "dumpConfig"
#define ATT_FRONT                   "front"
#define ATT_WEBPORT                 "webPort"
#define ATT_PROXYPORT               "proxyPort"
#define ATT_NEBELPORT               "nebelPort"
#define ATT_FRONTNAME               "frontName"
// ----------------------------- CFG DEFAULT VALUES ------
#define CFG_TRUE                    "true"
#define CFG_DEFAULT                 "Default"
#define CFG_ON                      "on"
#define CFG_OFF                     "off"
#define CFG_INTERNAL                "internal"
#define CFG_LITE                    "lite"
#define CFG_DOCKER                  "docker"
#define CFG_FRONTNAME               "my-angular-app"
#define CFG_WEBPORT                 "3000"
#define CFG_PROXYPORT               "8081"
#define CFG_NEBELPORT               "8080"

// -------------------------------------------------------
#define IS_CONFIG( A, B) (config->cfg(A)==B)
#define IS_NOT_CONFIG( A, B) (config->cfg(A)!=B)

// --- PARSER
#define TK_CONFIG "Config"

#define DUMP_CONFIGURATION { \
    cout << "Using: "<<cfg(ATT_USE_CONFIG)<<"\n"; \
    cout << "Configurations:\n"; \
    for (std::map<string,Config*>::iterator cfgItem = configurations.begin(); cfgItem != configurations.end(); ++cfgItem){ \
        std::cout << "\t" << (cfgItem->first) << "\n"; \
        for (std::map<string,string>::iterator attItem = cfgItem->second->attributes.begin(); attItem != cfgItem->second->attributes.end(); ++attItem){ \
            if ((attItem->first!=ATT_CLASSNAME)&&(attItem->first!=ATT_NAME)) \
                std::cout << "\t\t" << (attItem->first) << " : " << (attItem->second) << "\n"; \
        } \
    } \
}

class Config : public Component {
    private:
        void init();
        std::map<string, Config*> configurations;
        Config* config; // Can be use Config like Core
        Config(Lexical*, Component*);
        void loadConfig(string file);

    public:
        string fileName;
        string newFileName;

        Config(string file);

        void saveConfig(string file, string script);

        void parse();
        void parseConfig();

        string cfg(string key, string defValue = "");

        virtual int getObjectNum();
        virtual string doQuit();
};
#endif // !defined( CONFIG_H )