#include "includes.h"

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
        request(beginQuerry);
    };
private:
    int request(std::string querry){
        sqlite3_stmt* stmt;
        sqlite3_prepare(db, querry.c_str(), querry.size(), &stmt, NULL);
        if(sqlite3_step(stmt) != SQLITE_DONE) {
            return 1;
        }
        return 0;
    }
public:
    int login(std::string username, std::string passwd){
        unsigned char* temp;
        int temp_len;
        if(encrypt((unsigned char*)passwd.c_str(), passwd.size(), key, (unsigned char*)"randivecfrommine", temp, temp_len)){
            return 1;
        }
    }
};