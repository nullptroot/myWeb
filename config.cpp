#include "config.h"
bool Config::parse_config_file(const std::string &configFile)
{
    Json::Reader reader;
	Json::Value root;
	std::ifstream in(configFile, std::ios::binary);
	if( !in.is_open() )  
	{ 
	    std::cout << "Error opening config file"<<std::endl;; 
	    return false; 
	}
	if(reader.parse(in,root))
	{
        PORT = root["PORT"].asInt();
        LOGWrite = root["LOGWrite"].asInt();
        TRIGMode = root["TRIGMode"].asInt();
        LISTENTrigmode = root["LISTENTrigmode"].asInt();
        CONNTrigmode = root["CONNTrigmode"].asInt();
        OPT_LINGER = root["OPT_LINGER"].asInt();
        sql_num = root["sql_num"].asInt();
        thread_num = root["thread_num"].asInt();
        close_log = root["close_log"].asInt();
        actor_model = root["actor_model"].asInt();
        user = root["user"].asString();
        password = root["passwd"].asString();
        databaseName = root["databasename"].asString();
	}
	else
	{
	    std::cout << "Error parse config file\n" << std::endl;
        return false;	
	}
	in.close();
    return true;
}