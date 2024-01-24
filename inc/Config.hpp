
#if !defined( CONFIG_H )
#define CONFIG_H

#include "../inc/Component.hpp"
#include "../inc/Lexical.hpp"

// -------------------------------------------------------
#define CONFIG_FILE "./Config.nebel"
// ---------------------------------- ATTRIBUTES ---------
#define ATT_DISABLE_PARSE           "disableParse"
#define ATT_DISABLE_SERVER          "disableServer"
#define ATT_USE_CONFIG              "useConfig"
#define ATT_DUMP_CONFIG             "dumpConfig"
#define ATT_SERVERPORT              "serverPort"
// ---------------------------------- CFG VALUES ---------
#define CFG_TRUE                    "true"
#define CFG_DEFAULT                 "Default"
#define CFG_DEFAULT_PORT            "8080"
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
        
    public:
        string fileName;
        string newFileName;

        Config(Lexical*, Component*);
        void loadConfig(string file);
        void saveConfig(string file, string script);

        void parse();
        void parseConfig();

        string cfg(string key, string defValue = "");

        virtual int getObjectNum();
        virtual string doQuit();
};
#endif // !defined( CONFIG_H )