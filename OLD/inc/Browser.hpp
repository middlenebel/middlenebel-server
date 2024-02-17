#if !defined( BROWSER_H )
#define BROWSER_H

#include <iostream>

using namespace std;

class Browser : public Component{
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