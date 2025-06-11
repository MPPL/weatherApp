#ifndef DB_H_
#define DB_H_
#include "crypt.h"
#include "basic.h"

class DBManager{
    sqlite3* db;
    std::string DBname;
    std::filesystem::path dbPath;
    std::filesystem::path keyFolder;
    key_iv* prvkey;
public:
    DBManager(std::string db_name);
    int login(std::string username, std::string passwd);
    int register_user(std::string username, std::string passwd);
private:
    void first_key_setup();
    void first_setup();
    int request(std::string querry, int (*callback)(void**, int, int, int[], void*), void* struc);
    static int emptyCallback(void** data, int row_count, int col_count, int* datatypes, void* out);
    static int user_exists(void** data, int row_count, int col_count, int* datatypes, void* out);
};

#endif