#ifndef DB_H_
#define DB_H_
#include "includes.h"
#endif

class DBManager{
    sqlite3* db;
    std::string DBname;
    unsigned char* key;
    bool first_setup = false;
public:
    DBManager(std::string db_name);
    int login(std::string username, std::string passwd);
    int register_user(std::string username, std::string passwd);
private:
    int request(std::string querry, int (callback)(void*, int, char**, char**), void* struc);
    static int user_exists(void* vu, int argc, char** argv, char** ColName);
};