#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>


class TryCppHttpLib{
    // HTTP
    httplib::Server svr;

    // HTTPS
    httplib::SSLServer svr;

    int tryLib(){
        svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
        });

        svr.listen("0.0.0.0", 8080);
    }
};
