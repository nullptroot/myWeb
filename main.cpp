#include "config.h"
#include <iostream>

int main(int argc,char *argv[])
{
    if(argc < 2)
    {
        std::cout<<"usage "<<argv[0]<<" configFile.json"<<std::endl;
        return 1;
    }
    Config config;
    config.parse_config_file(argv[1]);
    
    WebServer server;
    
    server.init(config.PORT,config.user,config.password,config.databaseName,config.LOGWrite,
                    config.OPT_LINGER,config.TRIGMode,config.sql_num,
                    config.thread_num,config.close_log,config.actor_model);
    
    server.log_write();
    

    server.sql_pool();
    

    server.thread_pool();
    

    server.trig_mode();
    

    server.eventListen();
    

    server.eventLoop();
    

    return 0;
}