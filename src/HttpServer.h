#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include "../include/httplib.h"
#include <jsoncpp/json/json.h>
#include <string>
class HttpServer
{
public:
    void Run();
    HttpServer();
    ~HttpServer();
};

#endif