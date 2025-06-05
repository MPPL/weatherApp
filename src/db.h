#ifndef DB_H_
#define DB_H_
#include "includes.h"
#endif

class DBManager{
public:
    DBManager(std::string db_name);
    int login(std::string username, std::string passwd);
    int register_user(std::string username, std::string passwd);
private:
    int request(std::string querry);
};