#if !defined( BROWSER_H )
#define BROWSER_H

#include <iostream>
#include "wrappers/HttpServer.hpp"

using namespace std;

class Browser{
    public:
        string fileName;
        string newFileName;

        Browser();

        void load(string file);
        void save(string file, string script);

        string getBrowserReload(string base);
        string doBrowserAction(string action);
};
#endif