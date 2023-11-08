
#include "log11.h"
#define LOG_VAL_PARM 1024

Log::Log()
{
    m_count = 0;
    m_is_async = false;
}
Log::~Log()
{
    m_fp.close();
}

bool Log::init(const std::string &file_name,int close_log,int log_buf_size,int split_lines,int max_queue_size)
{
    if(max_queue_size >= 1)
    {
        m_is_async = true;
        m_log_queue.reset(new block_queue<std::string>(max_queue_size));
        std::thread tid = std::thread(&Log::flush_log_thread,this);
        tid.detach();
    }
    map.emplace_back("[debug]:");
    map.emplace_back("[info]:");
    map.emplace_back("[warn]:");
    map.emplace_back("[error]:");

    m_closs_log = close_log;
    m_log_buf_size = log_buf_size;
    m_buf.resize(m_log_buf_size);
    m_split_lines = split_lines;

    auto tim = std::chrono::system_clock::now();
    
    time_t t = std::chrono::system_clock::to_time_t(tim);
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    int index = file_name.find('/');
    log_name = file_name.substr(index+1);
    std::string log_full_name;
    if(index == log_full_name.size())
        log_full_name += std::to_string(my_tm.tm_year+1900)
                        +"_"+std::to_string(my_tm.tm_mon+1)
                        +"_"+std::to_string(my_tm.tm_mday);
    else
    {
        dir_name = file_name.substr(0,index+1);
        log_full_name += dir_name + std::to_string(my_tm.tm_year+1900)
                        +"_"+std::to_string(my_tm.tm_mon+1)
                        +"_"+std::to_string(my_tm.tm_mday)
                        +"_"+ log_name;
    }
    
    m_today = my_tm.tm_mday;

    m_fp.open(log_full_name,std::fstream::out);
    

    if(!m_fp.is_open())
        return false;
    return true;
}
void Log::flush(void)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    m_fp.flush();
}