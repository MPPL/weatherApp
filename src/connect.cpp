#include "includes.h"

Connection::Connection(){
    data.memory = (char*)malloc(1);
    data.size = 0;
}

void Connection::reset(){
    free(data.memory);
    data.memory = (char*)malloc(1);
    data.size = 0;    
}

//Memory Handler for accessing data from requests
size_t Connection::MemoryHandler(void* content, size_t size, size_t mem, void* user){
    size_t SIZE = size * mem;
    simpledata* tmp = (simpledata*) user;

    char* ptr = (char*)realloc(tmp->memory, tmp->size+SIZE+1);
    if(ptr == NULL){
        std::cout << "OOM: Out Of Memory\n";
        return 0;
    }

    tmp->memory = ptr;
    memcpy(&(tmp->memory[tmp->size]), content, SIZE);
    tmp->size += SIZE;
    tmp->memory[tmp->size] = 0;

    return SIZE;
}


//Function for getting latitude and longitude of a city
geoloc Connection::Geolocation(std::string name){
    curl_handle = curl_easy_init();
    if(!curl_handle){
        exit(-1);
    }

    std::string temp = std::format("https://geocoding-api.open-meteo.com/v1/search?name={}&count=1&language=en&format=json", name);

    data.size = 0;

    curl_easy_setopt(curl_handle, CURLOPT_URL, temp.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1l);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, MemoryHandler);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    urlcode = curl_easy_perform(curl_handle);

    if(urlcode != CURLE_OK){
        std::cout << "CURL ERROR: " << curl_easy_strerror(urlcode) << "\n";
    }

    curl_easy_cleanup(curl_handle);

    nlohmann::json json = nlohmann::json::parse(data.memory);
    return (geoloc){(float)json["results"][0]["latitude"], (float)json["results"][0]["longitude"]};
}

//curl request to open-meteo
simpledata Connection::request(std::vector<std::string> params, int paramsLen){
    curl_handle = curl_easy_init();
    if(!curl_handle){
        exit(-1);
    }

    url = "https://api.open-meteo.com/v1/forecast?";
    for(int i = 0; i < paramsLen -1; i++){
        url = std::format("{}{}&", url, params[i]);
    }
    if(paramsLen){
        url = std::format("{}{}", url, params[paramsLen-1]);
    }

    std::cout << url << "\n";

    data.size = 0;

    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1l);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, MemoryHandler);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&data);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    urlcode = curl_easy_perform(curl_handle);

    if(urlcode != CURLE_OK){
        std::cout << "CURL ERROR: " << curl_easy_strerror(urlcode) << "\n";
    }
    if(debug){
        std::cout << "Size: " << data.size << "\n";
        std::cout << "Data: " << (std::string)data.memory << "\n";
    }

    curl_easy_cleanup(curl_handle);
    return data;
}

//scrapped
std::string Connection::buildParam(std::string name, std::string value){
    return std::format("{}={}", name, value);
}