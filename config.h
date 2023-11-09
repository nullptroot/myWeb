#ifndef CONFIG_H
#define CONFIG_H
#include "./dist/json/json.h"
#include "./dist/json/json-forwards.h"
#include "webserver.h"

class Config
{
    public:
    /*改为初始化列表形式了*/
        Config(){};
        ~Config(){};
        bool parse_config_file(const std::string &configFile);
        int PORT;
        int LOGWrite;
        int TRIGMode;
        int LISTENTrigmode;
        int CONNTrigmode;
        int OPT_LINGER;
        int sql_num;
        int thread_num;
        int close_log;
        int actor_model;
        std::string user;
        std::string password;
        std::string databaseName;

};
#endif