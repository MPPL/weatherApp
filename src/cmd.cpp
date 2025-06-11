#include "includes.h"

CMDpage CMDpage_con(unsigned int id, void (DrawFunc)(nlohmann::json*), bool (InputFunc)(int*, const char*, DBManager*, Connection*, nlohmann::json*)){
    CMDpage ret = CMDpage(id, DrawFunc, InputFunc);
    std::cout << id << " < id of page\n";
    return ret;
}

CMD::CMD(int startingPage, CMDpage pageList[], DBManager* dbptr, Connection* conptr){
    int pageCounter = startingPage;
    pages = pageList;
    db = dbptr;
    con = conptr;
    last_data = nlohmann::json();
}

//Draw and Input of a Page
bool CMD::Step(){
    std::cout << "-------------------------------\n";
    pages[pageCounter].Draw(&last_data);
    std::cout << error << '\n';
    return pages[pageCounter].Input(&pageCounter, error, db, con, &last_data);
}