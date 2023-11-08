#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>
#include "block_queue11.h"
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include <thread>
#include <chrono>
#include <unistd.h>
class Log
{
    private:
        std::string dir_name;
        std::string log_name;
        std::vector<std::string> map;
        int m_split_lines;
        int m_log_buf_size;
        long long m_count;
        int m_today;
        std::fstream m_fp;
        /*原代码根本就没有释放啊 内存泄露了*/
        std::string m_buf;
        /*原代码根本就没有释放啊 内存泄露了*/
        std::unique_ptr<block_queue<std::string>> m_log_queue;
        bool m_is_async;
        std::mutex m_mutex;
        int m_closs_log;
    public:
        static Log *get_instance()
        {
            static Log instance;
            return &instance;
        }
        void flush_log_thread()
        {
            Log::get_instance()->async_write_log();
        }
        bool init(const std::string &file_name,int close_log,int log_buf_size = 8192,
                    int split_lines = 5000000,int max_queue_size = 0);
        /*展开参数的函数，对result进行赋值 必须使用编译期if判断才可以*/
        template <typename T,typename... Args>
        void fun(std::string &result,const T &t,const Args&... args)
        {
            if constexpr (std::is_constructible<std::string,T>::value)
                result += t;
            else
                result += std::to_string(t);
            if constexpr (sizeof...(args) >= 1)
                fun(result,args...);
        }
        /*把c语言的变长参数变成了 模板变长参数*/
        template <typename T,typename... Args>
        void write_log(int level,const T &val,const Args&... args)
        {
            struct timeval now = {0,0};
            gettimeofday(&now,nullptr);
            time_t t = now.tv_sec;
            struct tm *sys_tm = localtime(&t);
            struct tm my_tm = *sys_tm;
            std::unique_lock<std::mutex> lk(m_mutex);
            m_count++;

            if(m_today != my_tm.tm_mday || m_count % m_split_lines == 0)
            {
                /*新日志名字*/
                std::string new_log;
                m_fp.flush();
                m_fp.close();

                std::string tail = std::to_string(my_tm.tm_year+1900)+"_"
                                    +std::to_string(my_tm.tm_mon+1)+"_"
                                    +std::to_string(my_tm.tm_mday) +"_";

                if(m_today != my_tm.tm_mday)
                {
                    new_log = dir_name+tail+log_name;
                    m_today = my_tm.tm_mday;
                    m_count = 0;
                }
                else
                    new_log = dir_name+tail+log_name + std::to_string(m_count/m_split_lines);
                m_fp.open(new_log,std::fstream::out);
            }
            lk.unlock();

            std::string log_str;
            lk.lock();
            m_buf.clear();
            /*这里使得每条日志都带上了时间*/
            m_buf += std::to_string(my_tm.tm_year + 1900)+'-'
                        +std::to_string(my_tm.tm_mon + 1)+'-'
                        +std::to_string(my_tm.tm_mday)+' '
                        +std::to_string(my_tm.tm_hour)+':'
                        +std::to_string(my_tm.tm_min)+':'
                        +std::to_string(my_tm.tm_sec)+'.'
                        +std::to_string(now.tv_usec)+' '
                        +map[level] + ' ';
            fun(m_buf,val,args...);
            log_str = m_buf;

            lk.unlock();

            if(m_is_async && !m_log_queue->full())
                m_log_queue->push(log_str);
            else
            {
                lk.lock();
                m_fp<<log_str<<std::endl;
                lk.unlock();
            }
        }
        void flush(void);
    private:
        Log();
        virtual ~Log();
        void *async_write_log()
        {
            std::string single_log;
            while(m_log_queue->pop(single_log))
            {
                std::lock_guard<std::mutex> lk(m_mutex);
                m_fp<<single_log<<std::endl;
            }
            return nullptr;
        }

};
/*
LOG_DEBUG(format, ...)
展开为如下，感觉不如内联函数好用
if(0 == m_closs_log)
{
    Log::get_instance()->write_log(0,format,##__VA_ARGS__);
    Log::get_instance()->flush();
}
*/
#define LOG_DEBUG(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(0,format,##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_INFO(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(1,format,##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_WARN(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(2,format,##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_ERROR(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(3,format,##__VA_ARGS__); Log::get_instance()->flush();}

#endif