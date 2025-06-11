#ifndef CONNEC_H_
#define CONNEC_H_
#include "basic.h"
#include "crypt.h"
#include "db.h"

typedef struct {float lat; float lon;} geoloc;

typedef struct {char* memory; unsigned int size;} simpledata;

class Connection{
    public:
    
    bool debug = false;
    std::string url;
    CURL *curl_handle;
    CURLcode urlcode;
    simpledata data;

    Connection();

    void reset();

    static size_t MemoryHandler(void* content, size_t size, size_t mem, void* user);

    geoloc Geolocation(std::string name);

    simpledata request(std::vector<std::string> params, int paramsLen);

    std::string buildParam(std::string name, std::string value);
};
#endif