#include "preps.h"

void IntroPageDraw(nlohmann::json* data){
    std::cout << "1. Login\n2. Register\n3. Exit\n\n";
}
bool IntroPageInput(int* pageCounter, const char* error, DBManager* db, Connection* con, nlohmann::json* last_data){
    
    // technically, login and register exists, just database refuses to work any way, and I don't understand why
    //  INSERTS doesn't work so... i have no way to check it if it works :/
    //  the statements are correct
    //  no error is thrown
    //  just does nothing, ignores or doesn't save

    // so here I just skip whole login and go straight to weather forecast
    *pageCounter = 3;
    return true;
    
    /*
    int temp = 0;
    std::cout << ": ";
    std::cin >> temp;
    switch(temp){
        case 1:{
            error = "";
            *pageCounter = 1;
            break;
        }
        case 2:{
            error = "";
            *pageCounter = 2;
            break;
        }
        case 3:{
            error = "";
            return false;
        }
        default:{
            error = "Error, wrong option provided";
            break;
        }
    }
    return true;*/
}

void EmptyDraw(nlohmann::json* data){
    return;
}
bool LoginInput(int* pageCounter, const char* error, DBManager* db, Connection* con, nlohmann::json* last_data){
    //have to draw here
    std::string username;
    std::string passwd;
    std::cout << "\nLOGIN\n\nusername: ";
    std::cin >> username;
    std::cout << "\npasswd: ";
    std::cin >> passwd;
    if(db->login(username, passwd) != 0){
        error = "something went wrong!";
        *pageCounter = 0;
    }else{
        *pageCounter = 3;
        error = "";
    }
    return true;
}

bool RegisterInput(int* pageCounter, const char* error, DBManager* db, Connection* con, nlohmann::json* last_data){
    //have to draw here
    std::string username;
    std::string passwd;
    std::cout << "\nREGISTER\n\nusername: ";
    std::cin >> username;
    std::cout << "\npasswd: ";
    std::cin >> passwd;
    if(!db->login(username, passwd)){
        error = "something went wrong!";
        *pageCounter = 0;
    }else{
        *pageCounter = 3;
        error = "";
    }
    return true;
}

void MenuDraw(nlohmann::json* data){
    std::cout << "1. Today Simple Weather\n2. 3-Day Simple Weather\n3. 7-Day Simple Weather\n4. 7-Dat Advanced Weather\n5. Exit\n\n";
}

bool MenuInput(int* pageCounter, const char* error, DBManager* db, Connection* con, nlohmann::json* last_data){
    int temp = 0;
    std::cout << ": ";
    std::cin >> temp;
    switch(temp){
        case 1:{
            std::string temp2;
            std::cout << "\nLocation: ";
            std::cin >> temp2;
            geoloc temp3 = con->Geolocation(temp2);
            std::vector<std::string> temp4 = {
                std::format("latitude={}", std::to_string(temp3.lat)),
                std::format("longitude={}", std::to_string(temp3.lon)),
                "current=relative_humidity_2m,temperature_2m,precipitation,wind_speed_10m",
            };
            simpledata data = con->request(temp4, 3);
            *last_data = nlohmann::json::parse(data.memory);
            *pageCounter = 4;
            break;
        }
        case 2:{
            std::string temp2;
            std::cout << "\nLocation: ";
            std::cin >> temp2;
            geoloc temp3 = con->Geolocation(temp2);
            std::vector<std::string> temp4 = {
                std::format("latitude={}", std::to_string(temp3.lat)),
                std::format("longitude={}", std::to_string(temp3.lon)),
                "daily=temperature_2m_max,temperature_2m_min,precipitation_sum,wind_speed_10m_max",
                "forecast_days=3",
            };
            simpledata data = con->request(temp4, 4);
            *last_data = nlohmann::json::parse(data.memory);
            *pageCounter = 5;
            break;
        }
        case 3:{
            std::string temp2;
            std::cout << "\nLocation: ";
            std::cin >> temp2;
            geoloc temp3 = con->Geolocation(temp2);
            std::cout << "debug+\n";
            std::vector<std::string> temp4 = {
                std::format("latitude={}", std::to_string(temp3.lat)),
                std::format("longitude={}", std::to_string(temp3.lon)),
                "daily=temperature_2m_max,temperature_2m_min,precipitation_sum,wind_speed_10m_max",
                "forecast_days=7",
            };
            std::cout << "debug+\n";
            simpledata data = con->request(temp4, 4);
            *last_data = nlohmann::json::parse(data.memory);
            *pageCounter = 6;
            break;
        }
        case 4:{
            std::string temp2;
            std::cout << "\nLocation: ";
            std::cin >> temp2;
            geoloc temp3 = con->Geolocation(temp2);
            std::vector<std::string> temp4 = {
                std::format("latitude={}", std::to_string(temp3.lat)),
                std::format("longitude={}", std::to_string(temp3.lon)),
                "daily=temperature_2m_max,temperature_2m_min,precipitation_sum,wind_speed_10m_max",
                "forecast_days=7",
            };
            simpledata data = con->request(temp4, 4);
            *last_data = nlohmann::json::parse(data.memory);
            *pageCounter = 7;
            break;
        }
        case 5:{
            return false;
        }
        default:{
            error = "Wrong input provided!";
            break;
        }
    }
    return true;
}

void W1Draw(nlohmann::json* data){
    std::cout << "\nlatitude: " << (*data)["latitude"];
    std::cout << "\nlongitude: " << (*data)["longitude"];
    std::cout << "\ntemperature: " << (*data)["current"]["temperature_2m"] << "C";
    std::cout << "\nhumidity: " << (*data)["current"]["relative_humidity_2m"] << "%";
    std::cout << "\nprecipitation: " << (*data)["current"]["precipitation"] << "mm";
    std::cout << "\nwind speed: " << (*data)["current"]["wind_speed_10m"] << "km/h";
    std::cout << "\n\n1. Menu";
    std::cout << "\n2. Exit\n\n";
}

bool W1Input(int* pageCounter, const char* error, DBManager* db, Connection* con, nlohmann::json* last_data){
    int temp = 0;
    std::cout << ": ";
    std::cin >> temp;
    switch(temp){
        case 1:{
            error = "";
            *pageCounter = 3;
            break;
        }
        case 2:{
            error = "";
            return false;
        }
        default:{
            error = "Wrong input provided!";
            break;
        }
    }
    return true;
}

void W3Draw(nlohmann::json* data){
    std::cout << "\nlatitude: " << (*data)["latitude"];
    std::cout << "\nlongitude: " << (*data)["longitude"];
    for(int i = 0; i < 3; i++){
        std::cout << "\nDAY 1";
        std::cout << "\ntemperature (max): " << (*data)["daily"]["temperature_2m_max"][i] << "C";
        std::cout << "\ntemperature (min): " << (*data)["daily"]["temperature_2m_min"][i] << "C";
        std::cout << "\nprecipitation: " << (*data)["daily"]["precipitation_sum"][i] << "mm";
        std::cout << "\nwind speed: " << (*data)["daily"]["wind_speed_10m_max"][i] << "km/h";
    }
    std::cout << "\n\n1. Menu";
    std::cout << "\n2. Exit";
    std::cout << "\n2. Exit\n\n";
}

bool W3Input(int* pageCounter, const char* error, DBManager* db, Connection* con, nlohmann::json* last_data){
    int temp = 0;
    std::cout << ": ";
    std::cin >> temp;
    switch(temp){
        case 1:{
            error = "";
            *pageCounter = 3;
            break;
        }
        case 2:{
            error = "";
            return false;
        }
        default:{
            error = "Wrong input provided!";
            break;
        }
    }
    return true;
}

void W7Draw(nlohmann::json* data){
    std::cout << "\nlatitude: " << (*data)["latitude"];
    std::cout << "\nlongitude: " << (*data)["longitude"];
    for(int i = 0; i < 7; i++){
        std::cout << "\nDAY 1";
        std::cout << "\ntemperature (max): " << (*data)["daily"]["temperature_2m_max"][i] << "C";
        std::cout << "\ntemperature (min): " << (*data)["daily"]["temperature_2m_min"][i] << "C";
        std::cout << "\nprecipitation: " << (*data)["daily"]["precipitation_sum"][i] << "mm";
        std::cout << "\nwind speed: " << (*data)["daily"]["wind_speed_10m_max"][i] << "km/h";
    }
    std::cout << "\n\n1. Menu";
    std::cout << "\n2. Exit\n\n";
}

bool W7Input(int* pageCounter, const char* error, DBManager* db, Connection* con, nlohmann::json* last_data){
    int temp = 0;
    std::cout << ": ";
    std::cin >> temp;
    switch(temp){
        case 1:{
            error = "";
            *pageCounter = 3;
            break;
        }
        case 2:{
            error = "";
            return false;
        }
        default:{
            error = "Wrong input provided!";
            break;
        }
    }
    return true;
}

void W7ADraw(nlohmann::json* data){
    std::cout << "\nlatitude: " << (*data)["latitude"];
    std::cout << "\nlongitude: " << (*data)["longitude"];
    for(int i = 0; i < 7; i++){
        std::cout << "\n\n  -- DAY " << i << " --  ";
        std::cout << "\ntemperature (max): " << (*data)["daily"]["temperature_2m_max"][i] << "C";
        std::cout << "\ntemperature (min): " << (*data)["daily"]["temperature_2m_min"][i] << "C";
        std::cout << "\nprecipitation: " << (*data)["daily"]["precipitation_sum"][i] << "mm";
        std::cout << "\nwind speed: " << (*data)["daily"]["wind_speed_10m_max"][i] << "km/h";
    }
    std::cout << "\n\n1. Menu";
    std::cout << "\n2. Exit\n\n";
}

bool W7AInput(int* pageCounter, const char* error, DBManager* db, Connection* con, nlohmann::json* last_data){
    int temp = 0;
    std::cout << ": ";
    std::cin >> temp;
    switch(temp){
        case 1:{
            error = "";
            *pageCounter = 3;
            break;
        }
        case 2:{
            error = "";
            return false;
        }
        default:{
            error = "Wrong input provided!";
            break;
        }
    }
    return true;
}


// pages of menu we can see in cli
CMDpage pages[] = {
    CMDpage_con(1, IntroPageDraw, IntroPageInput),
    CMDpage_con(2, EmptyDraw, LoginInput),
    CMDpage_con(3, EmptyDraw, RegisterInput),
    CMDpage_con(4, MenuDraw, MenuInput),
    CMDpage_con(5, W1Draw, W1Input),
    CMDpage_con(6, W3Draw, W3Input),
    CMDpage_con(7, W7Draw, W7Input),
    CMDpage_con(8, W7ADraw, W7AInput),
};

/*
CMDparam paramList[] = {
    CMDparam(1, "latitude", ParamFloat, InputFloat),
    CMDparam(2, "longitude", ParamFloat, InputFloat),
    CMDparam(3, "elevation", ParamFloat, InputFloat),
    CMDparam(4, "hourly", ParamText, InputText),
    CMDparam(5, "daily", ParamText, InputText),
    CMDparam(6, "current", ParamText, InputText),
    CMDparam(7, "temperature_unit", ParamText, InputText),
    CMDparam(8, "wind_speed_unit", ParamText, InputText),
    CMDparam(9, "precipitation_unit", ParamText, InputText),
    //CMDparam(10, "timeformat", ParamText, InputText),
    //CMDparam(11, "timezone", ParamText, InputText),
    CMDparam(10, "past_days", ParamInt, InputInt),
    CMDparam(11, "forecast_days", ParamInt, InputInt),
    //CMDparam(14, "forecast_hours", ParamInt, InputInt),
    //CMDparam(15, "forecast_minutely_15", ParamInt, InputInt),
    //CMDparam(16, "past_hours", ParamInt, InputInt),
    //CMDparam(17, "past_minutely_15", ParamInt, InputInt),
    //CMDparam(18, "start_date", ParamText, InputText),
    //CMDparam(19, "end_date", ParamText, InputText),
    //CMDparam(20, "start_hour", ParamText, InputText),
    //CMDparam(21, "end_hour", ParamText, InputText),
    //CMDparam(22, "start_minutely_15", ParamText, InputText),
    //CMDparam(23, "end_minutely_15", ParamText, InputText),
    //CMDparam(24, "models", ParamText, InputText),
    //CMDparam(25, "cell_selection", ParamText, InputText),
    //CMDparam(26, "apikey", ParamText, InputText),
};
*/