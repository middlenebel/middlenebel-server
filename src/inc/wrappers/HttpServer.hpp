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
#if !defined( HTTPSERVER_H )
#define HTTPSERVER_H

// #define CPPHTTPLIB_OPENSSL_SUPPORT //libssl and libcrypto should be linked.
#include <std_function.h>
#include <httplib.h>

typedef httplib::Request Request;
typedef httplib::Response Response;
class HttpServer{
    // HTTP
    httplib::Server svr;
    // HTTPS httplib::SSLServer svr;
    public:

    using Handler = std::function<void(const Request &, Response &)>;

    bool listen(const std::string host, int port){  return svr.listen(host, port); }
    
    HttpServer& Get(const std::string &pattern, Handler handler) {
      svr.Get( pattern, handler);
      return *this;
    }
    HttpServer& Post(const std::string &pattern, Handler handler) {
      svr.Post( pattern, handler);
      return *this;
    }
    void stop(){
      svr.stop();
    }

    int tryLib(){
        svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
        });

        svr.listen("0.0.0.0", 8080);
        return 0;
    }
};

#endif

