#include "src/includes.h"
using json = nlohmann::json;

typedef struct {char* memory; unsigned int size;} simpledata;
typedef struct {
    unsigned long long use_flag = 0;
    double latitude;
    double longitude;
    float elevation;
    std::vector<std::string> hourly;
    std::vector<std::string> daily;
    std::vector<std::string> current;
    std::string temperature_unit = "celcius";
    std::string wind_speed_unit = "kmh";
    std::string precipation_unit = "mm";
    std::string timezone = "iso8601";
    char past_days = 0;
    char forecast_days = 7;
    std::string start_date;
    std::string end_date;
    std::vector<std::string> models = {"auto"};
    std::string cell_selection = "land";
} weatherSettings;

class Handler{
    public:
    
    bool debug = false;
    char* url;
    CURL *curl_handle;
    CURLcode urlcode;
    simpledata data;

    weatherSettings Settings;

    Handler(){

        data.memory = (char*)malloc(1);
        data.size = 0;
    }

    void reset(){
        free(data.memory);
        data.memory = (char*)malloc(1);
        data.size = 0;    
    }

    static size_t MemoryHandler(void* content, size_t size, size_t mem, void* user){
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

    simpledata request(){
        curl_handle = curl_easy_init();
        if(!curl_handle){
            exit(-1);
        }

        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
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
};

void printData(json data){
    std::cout << "latitude: " << data["latitude"] << "\n";
    std::cout << "longitude: " << data["longitude"] << "\n";
    std::cout << "is_day: " << (bool)(int)(data["current"]["is_day"]) << "\n";
}

#define CMD_MODE
void cmd(Handler* h, DBManager* db){
    int option = 0;
    bool loop = true;
    while(loop){
        std::cout << "1. Log in\n2. Register\n3. Exit";
        std::cin >> option;
        switch(option){
            case 1:{
                std::string user;
                std::string password;
                std::cout << "\n\nLoging in\nusername: ";
                std::cin >> user;
                std::cout << "\npassword: ";
                std::cin >> password;
                if(db->login(user, password) != 0){
                    std::cout << "\n\x1B[32mLogin Failed!\033[0m\n";
                }
                loop = false;
                break;
            }
            case 2:{
                std::string user;
                std::string password;
                std::cout << "\n\nRegister\nusername: ";
                std::cin >> user;
                std::cout << "\npassword: ";
                std::cin >> password;
                if(db->register_user(user, password) != 0){
                    std::cout << "\n\x1B[32mRegister Failed!\033[0m\n";
                }
                loop = false;
                break;
            }
            case 3:{
                exit(0);
                loop = false;
                break;
            }
        }
    }
}

int main(int argc, char* argv[]){

    Handler Hand = Handler();
    DBManager DB = DBManager("db/etherApp.db");
    
#ifdef CMD_MODE
    cmd(&Hand, &DB);
#endif

    //Hand.debug = true;
    Hand.url = "https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=is_day,apparent_temperature,relative_humidity_2m,temperature_2m&forecast_days=1";
    json data = json::parse(Hand.request().memory);
    printData(data);
    return 0;
}