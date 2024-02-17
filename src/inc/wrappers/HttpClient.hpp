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
#if !defined( HTTPCLIENT_H )
#define HTTPCLIENT_H

// #define CPPHTTPLIB_OPENSSL_SUPPORT //libssl and libcrypto should be linked.
#include <httplib.h>
// #include "../../../third_party/cpp-httplib/httplib.h"
#include <iostream>

// cli.Post("/ptz.cmd", "pan_left=50", "application/x-www-form-urlencoded");

class HttpClient : public httplib::Client{
    std::string port;

    public:
    
    explicit HttpClient(const std::string &scheme_host_port) : httplib::Client(scheme_host_port){
        std::cout << "HttpClient " << scheme_host_port << "\n";
        port = scheme_host_port;
    }

    class Result : public httplib::Result{
      public: 
        Result(httplib::Result r) : httplib::Result{ std::move( r ) } {};
        Result() = default;
    };

    //inline 
    Result Get(const std::string &path) { 
      std::cout << "HttpClient - Get " << path << "\n";
      if (HttpClient::Result resCli = httplib::Client::Get( path )) { 
        if (resCli->status == httplib::StatusCode::OK_200) {
          std::cout << "HttpClient - Get response: " << resCli.value().body << "\n";
          return resCli; 
        }
      }
      HttpClient::Result error; //TODO define error
      return ( error );
    }
    
    //inline 
    Result Post(const std::string &path, const std::string params) { 
      std::cout << "HttpClient - Post " << path << "\n";
      // return static_cast<Result>( httplib::Client::Get(path) );
      // cli.Post("/ptz.cmd", "pan_left=50", "application/x-www-form-urlencoded");
      //application/octet-stream
      if (HttpClient::Result resCli = httplib::Client::Post( path, params, "application/x-www-form-urlencoded")) { 
        if (resCli->status == httplib::StatusCode::OK_200) {
          std::cout << "HttpClient - Post response: " << resCli.value().body << "\n";
          return resCli; 
        }
      }
      HttpClient::Result error; //TODO define error
      return ( error );
    }

    int tryLib(){
        httplib::Client cli("localhost", 8080);

        if (httplib::Result res = cli.Get("/hi")) {
          if (res->status == httplib::StatusCode::OK_200) {
            std::cout << res->body << std::endl;
          }
        } else {
          auto err = res.error();
          std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
        return 0;
    }
    
};
#endif



