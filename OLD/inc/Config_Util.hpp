
#if !defined( CONFIG_UTIL_H )
#define CONFIG_UTIL_H

#define IS_CONFIG( A, B) (config->cfg(A)==B)
#define IS_NOT_CONFIG( A, B) (config->cfg(A)!=B)

#define DUMP_CONFIGURATION { \
    for (std::map<string,Config*>::iterator cfgItem = configurations.begin(); cfgItem != configurations.end(); ++cfgItem){ \
        std::cout << (cfgItem->first) << "\n"; \
        for (std::map<string,string>::iterator attItem = cfgItem->second->attributes.begin(); attItem != cfgItem->second->attributes.end(); ++attItem){ \
            std::cout << "\t" << (attItem->first) << " : " << (attItem->second) << "\n"; \
        } \
    } \
}

#endif // !defined( CONFIG_UTIL_H )