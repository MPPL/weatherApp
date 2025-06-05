#include "includes.h"

typedef struct dbcheck{
    void* ptr;
    bool status;
    dbcheck(void* n_ptr = nullptr, bool n_status = false){
        ptr = n_ptr;
        status = n_status;
    }
} dbcheck;

class DBManager{


    sqlite3* db;
    std::string DBname;
    unsigned char* key;

public:
    DBManager(std::string db_name){

        
        std::ifstream f("key.key");
        key = (unsigned char*)malloc(17);
        f.read((char*)key, sizeof(key) - 1);
        f.close();

        DBname = db_name;
        sqlite3_open(DBname.c_str(), &db);
        std::string beginQuerry = "CREATE TABLE IF NOT EXISTS clients (id INTEGER PRIMARY KEY, name VARCHAR(64) NOT NULL, passwd VARCHAR(128) NOT NULL, lastlogin DATE);";
        request(beginQuerry, user_exists, nullptr);
    };
private:
    static int user_exists(void* vu, int argc, char** argv, char** ColName){
        for(int i = 0; i < argc; i++){
            if(strcmp((char*)(((dbcheck*)vu)->ptr), argv[1]) == 0){
                ((dbcheck*)vu)->status = true;
                return 0;
            }
        }
        ((dbcheck*)vu)->status = false;
        return 0;
    }

    int request(std::string querry, int (callback)(void*, int, char**, char**), void* struc){
        sqlite3_stmt* stmt;
        char* errorMsg;
        sqlite3_exec(db, querry.c_str(), callback, struc, &errorMsg);
        return 0;
    }
public:
    int login(std::string username, std::string passwd){
        unsigned char* temp;
        int temp_len;
        if(encrypt((unsigned char*)passwd.c_str(), passwd.size(), key, (unsigned char*)"randivecfrommine", temp, temp_len)){
            return 1;
        }
        std::string loginQuerry;
        dbcheck dbc = dbcheck((void*)username.c_str());
        request(loginQuerry, user_exists, &dbc);
        if(dbc.status){
            return 0;
        }
        return 1;
    };
    //register is taken for depricated keyword heh
    int register_user(std::string username, std::string passwd){
        unsigned char* temp;
        int temp_len;
        if(encrypt((unsigned char*)passwd.c_str(), passwd.size(), key, (unsigned char*)"randivecfrommine", temp, temp_len)){
            return 1;
        }
        std::string registerQuerry;
        dbcheck dbc = dbcheck((void*)username.c_str());
        request(registerQuerry, user_exists, &dbc);
        if(dbc.status){
            return 0;
        }
        return 1;
    }
};