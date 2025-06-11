#include "includes.h"

DBManager::DBManager(std::string db_name){

    // get executable path (OS Dependent)

    #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    // not tested
    #include <windows.h>
    char temp[MAX_PATH];
    GetModuleFileNameA(NULL, temp, MAX_PATH);
    #elif defined(unix) || defined(__unix) || defined(__unix__)
    char temp[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", temp, PATH_MAX);
    temp[count] = '\0';
    #elif defined(__APPLE__) || defined(__MACH__)
    #include <mach-o/dyld.h>
    char temp[PATH_MAX];
    uint32_t bufsize = PATH_MAX;
    _NSGetExecutablePath(temp, &bufsize);
    #endif
    dbPath = std::filesystem::path(temp).parent_path() / "db" / db_name;
    keyFolder = std::filesystem::path(temp).parent_path();

    if(!std::filesystem::exists(dbPath)){
        first_setup();
        if(!std::filesystem::exists((std::filesystem::path)(keyFolder / "prv.key").c_str()) || !std::filesystem::exists((std::filesystem::path)(keyFolder / "prv.iv").c_str())){
            first_key_setup();
        }else{
            prvkey = (key_iv*)malloc(sizeof(key_iv));
            prvkey->key = (unsigned char*)malloc(33);
            int key_size = std::filesystem::file_size((std::filesystem::path)(keyFolder / "prv.key").c_str());
            std::ifstream prv;
            prv.open((std::filesystem::path)(keyFolder / "prv.key").c_str(), std::ios::binary | std::ios::in);
            prv.read((char*)prvkey->key, key_size);
            prv.close();
            prvkey->key[key_size] = '\0';

            prvkey->iv = (unsigned char*)malloc(17);
            key_size = std::filesystem::file_size((std::filesystem::path)(keyFolder / "prv.iv").c_str());

            prv.open((std::filesystem::path)(keyFolder / "prv.iv").c_str(), std::ios::binary | std::ios::in);
            prv.read((char*)prvkey->iv, key_size);
            prv.close();
            prvkey->iv[key_size] = '\0';
        };
    }else if(!std::filesystem::exists((std::filesystem::path)(keyFolder / "prv.key").c_str()) || !std::filesystem::exists((std::filesystem::path)(keyFolder / "prv.iv").c_str())){
        std::cerr << "Keyfile missing with existing database!!! exiting...\n";
        exit(1);
    }else{
        prvkey = (key_iv*)malloc(sizeof(key_iv));
        prvkey->key = (unsigned char*)malloc(33);
        int key_size = std::filesystem::file_size((std::filesystem::path)(keyFolder / "prv.key").c_str());
        std::ifstream prv;
        prv.open((std::filesystem::path)(keyFolder / "prv.key").c_str(), std::ios::binary | std::ios::in);
        prv.read((char*)prvkey->key, key_size);
        prv.close();
        prvkey->key[key_size] = '\0';

        prvkey->iv = (unsigned char*)malloc(17);
        key_size = std::filesystem::file_size((std::filesystem::path)(keyFolder / "prv.iv").c_str());

        prv.open((std::filesystem::path)(keyFolder / "prv.iv").c_str(), std::ios::binary | std::ios::in);
        prv.read((char*)prvkey->iv, key_size);
        prv.close();
        prvkey->iv[key_size] = '\0';
    };
};

//key generation when no key files and db are found
void DBManager::first_key_setup(){

    // Generating new private key
    std::cout << "First key setup...\n";

    prvkey = generate_key_pair();
    std::ofstream prv;
    prv.open((std::filesystem::path)(keyFolder / "prv.key").c_str(), std::ios::binary | std::ios::out);
    std::cout << (std::filesystem::path)(keyFolder / "prv.key").c_str() << " | " << prvkey->key << '\n';
    prv.write((char*)prvkey->key, 32);
    prv.close();
    prv.open((std::filesystem::path)(keyFolder / "prv.iv").c_str(), std::ios::binary | std::ios::out);
    std::cout << (std::filesystem::path)(keyFolder / "prv.iv").c_str() << " | " << prvkey->iv << '\n';
    prv.write((char*)prvkey->iv, 16);
    prv.close();

}

//table setup (I just want to say that this worked, and created table, just INSERTS doens't work)
void DBManager::first_setup(){
    int a = 0;
    request("CREATE TABLE clients (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(64) NOT NULL, passwd VARCHAR(128) NOT NULL, lastlogin DATE);", emptyCallback, &a);
}

int DBManager::emptyCallback(void** data, int row_count, int col_count, int* datatypes, void* out){
    return 0;
}

//request callback for login and register
int DBManager::user_exists(void** data, int row_count, int col_count, int* datatypes, void* out){

    std::cout << row_count << " | " << col_count << '\n';

    if(row_count == 1){
        *(bool*)out = true;
        return 0;
    }else if(row_count > 1){
        *(bool*)out = false;
        std::cerr << "found more than one users?!\n";
        return 1;
    }else{
        *(bool*)out = false;
        return 0;
    }

    return 0;
};

//really long request, propably works... looks like that cause sqlite3_exec was segfaulting without reason
int DBManager::request(std::string querry, int (*callback)(void**, int, int, int[], void*), void* struc){
    sqlite3_stmt* stmt;
    char** errorMsg;
    void** val;
    int* datatypes;
    if(sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK){
        const char* tail;
        int n = sqlite3_prepare_v2(db, querry.c_str(), querry.size(), &stmt, &tail);

        int col_count = sqlite3_column_count(stmt);
        datatypes = (int*)malloc(sizeof(int)*col_count);
        val = (void**)malloc(sizeof(void*)*col_count);
        for(int i = 0; i < col_count; i++){
            switch(sqlite3_column_type(stmt, i)){
                case SQLITE_INTEGER:{
                    val[i] = (int*)malloc(sizeof(int)*128); // assume max rows to read
                    ((int**)val)[i][0] = sqlite3_column_int(stmt, i);
                    datatypes[i] = SQLITE_INTEGER;
                    break;
                }
                case SQLITE_FLOAT:{
                    val[i] = (float*)malloc(sizeof(float)*128); // assume max rows to read
                    ((float**)val)[i][0] = (float)sqlite3_column_double(stmt, i);
                    datatypes[i] = SQLITE_FLOAT;
                    break;
                }
                case SQLITE3_TEXT:{
                    val[i] = (char**)malloc(sizeof(char*)*128); // assume max rows to read
                    ((char***)val)[i][0] = (char*)sqlite3_column_text(stmt, i);
                    datatypes[i] = SQLITE_TEXT;
                    break;
                }
                case SQLITE_BLOB:{
                    val[i] = (void*)malloc(sizeof(void*)*128); // assume max rows to read
                    ((void***)val)[i][0] = (void*)sqlite3_column_blob(stmt, i);
                    datatypes[i] = SQLITE_BLOB;
                    break;
                }
                case SQLITE_NULL:{
                    val[i] = (int*)malloc(sizeof(int)*128); // assume max rows to read
                    ((int**)val)[i][0] = sqlite3_column_int(stmt, i);
                    datatypes[i] = SQLITE_NULL;
                    break;
                }
            };
        }
        
        int c = 0;
        n = sqlite3_step(stmt);
        
        while(n != SQLITE_DONE && n != SQLITE_ERROR && c < 128){
            c++;
            for(int i = 0; i < col_count; i++){
                switch(sqlite3_column_type(stmt, i)){
                    case SQLITE_INTEGER:{
                        ((int**)val)[i][c] = sqlite3_column_int(stmt, i);
                        break;
                    }
                    case SQLITE_FLOAT:{
                        ((float**)val)[i][c] = (float)sqlite3_column_double(stmt, i);
                        break;
                    }
                    case SQLITE3_TEXT:{
                        ((char***)val)[i][c] = (char*)sqlite3_column_text(stmt, i);
                        break;
                    }
                    case SQLITE_BLOB:{
                        ((void***)val)[i][c] = (void*)sqlite3_column_blob(stmt, i);
                        break;
                    }
                    case SQLITE_NULL:{
                        ((int**)val)[i][c] = sqlite3_column_int(stmt, i);
                        break;
                    }
                };
                n = sqlite3_step(stmt);
            }
        };

        if(n != SQLITE_DONE){
            std::cerr << "Error while processing statement!\n";
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return callback(val, c, col_count, datatypes, struc);
    }else{
        std::cout << "Failed to open database! " << dbPath.c_str() << '\n'; 
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }
};

//login of a user
int DBManager::login(std::string username, std::string passwd){
    unsigned char* temp = (unsigned char*)malloc(512);
    int temp_len = 512;
    std::cout << "debug--\n";
    if(!encrypt((unsigned char*)passwd.c_str(), passwd.size(), prvkey, temp, &temp_len)){
        return 1;
    }
    std::cout << "debug--\n";
    temp[temp_len] = '\0';
    std::cout << "debug--\n";
    std::string loginQuerry = std::format("SELECT * FROM clients WHERE clients.name == \'{}\' AND clients.passwd == \'{}\';", username, (char*)temp);
    std::cout << loginQuerry << '\n';
    std::cout << "debug--\n";
    bool is_logged;
    std::cout << "debug--\n";
    request(loginQuerry, user_exists, &is_logged);
    std::cout << "debug--\n";
    if(is_logged){
        return 0;
    }
    std::cout << "debug--\n";
    return 2;
};

//register is taken for depricated keyword heh
int DBManager::register_user(std::string username, std::string passwd){
    unsigned char* temp = (unsigned char*)malloc(512);
    int temp_len = 512;
    if(encrypt((unsigned char*)passwd.c_str(), passwd.size(), prvkey, temp, &temp_len)){
        return 1;
    }
    temp = (unsigned char*)realloc(temp, (temp_len+1) * sizeof(unsigned char));
    temp[temp_len] = '\0';
    std::string user_exist_Querry = std::format("SELECT * FROM clients WHERE clients.name == \'{}\';", username, (char*)temp);
    bool do_exist;
    request(user_exist_Querry, user_exists, &do_exist);
    if(do_exist){
        return 2;
    }
    
    std::string registerQuerry = std::format("INSERT INTO clients (name, passwd) VALUES('{}', '{}');", username, (char*)temp);
    std::cout << registerQuerry << '\n';
    request(registerQuerry, emptyCallback, NULL);
    return 0;
};