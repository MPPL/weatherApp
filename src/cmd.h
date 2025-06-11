#ifndef CMD_H_
#define CMD_H_
#include "basic.h"
#include "crypt.h"
#include "db.h"
#include "connect.h"

typedef struct CMDpage{
    unsigned int id;
    void (*Draw)(nlohmann::json*);
    bool (*Input)(int*, const char*, DBManager*, Connection*, nlohmann::json*);
    CMDpage(unsigned int id, void (DrawFunc)(nlohmann::json*), bool (InputFunc)(int*, const char*, DBManager*, Connection*, nlohmann::json*)){
        this->id = id;
        Draw = DrawFunc;
        Input = InputFunc;
    }
} CMDpage;

CMDpage CMDpage_con(unsigned int id, void (DrawFunc)(nlohmann::json*), bool (InputFunc)(int*, const char*, DBManager*, Connection*, nlohmann::json*));

class CMD{
    int pageCounter;
    const char* error = "";
    DBManager* db;
    Connection* con;
    CMDpage* pages;
    nlohmann::json last_data;

public:

    CMD(int startingPage, CMDpage pageList[], DBManager* dbptr, Connection* conptr);

    bool Step();
};
#endif