#include <string>
#include <iostream>
#include <stdarg.h>
// using namespace std;
using std::string;
using std::is_same;
using std::is_constructible;
using std::cout;
using std::endl;

/*
与运行期 if 语句有所不同，编译期 if 语句中的判断条件必须是编译期常量。
与预处理期 if 语句有所不同，编译期 if 语句中被丢弃的分支仍然需要进行语法检查。

这不就可以代替vsnprintf的日志写入了吗*/
// void fun(string &s){}
template <typename T,typename... Args>
void f(int level,const T &t,const Args&... args)
{
    string s = "";
    fun(s,args);
    cout<<s<<endl;
}
template <typename T,typename... Args>
void fun(string &result,const T &t,const Args&... args)
{
    if constexpr (is_constructible<std::string,T>::value)
        result += t;
    else
        result += std::to_string(t);
    if constexpr (sizeof...(args) >= 1)
        fun(result,args...);
}
int main()
{
    string s = "";
    const auto& b = string("assas");

    fun(s,b,18,60,9.8);
    f(1,2,"wqwq",b);
    // f(s,"%s,%d,%d","dasda ",10,56);
    cout<<s<<endl;

}