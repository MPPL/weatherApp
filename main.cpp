#include "src/includes.h"
using json = nlohmann::json;

int main(int argc, char* argv[]){

    Connection Handler = Connection();
    DBManager DB("eatherApp.db");
    CMD cmd(1, pages, &DB, &Handler);

    //Hand.debug = true;
    //Hand.url = "https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=is_day,apparent_temperature,relative_humidity_2m,temperature_2m&forecast_days=1";
    while(cmd.Step());
    return 0;
}

/* TO DO

+ create keys relative to executable (crossplatform)
+ to uchieve crossplatformness, find a way to check on what system you are
+ finish options to choose (kind of done)
- check if database works (doesn't work)
+ finish generation of open-meteo link
- error handling for open-meteo (nah, too much)
- cmd Menu: ListDraw() --- scrapped
- db DBManager: put \ before all " and ' characters inside values passed to querries to db --- scrapped

I still have a time to add safe sql statements, but i'll let it be, as db refuses to work
*/