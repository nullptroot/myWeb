# myWeb

当然是根据大神改进了，尽可能地使用新标准的语法<br>实现了时间轮和时间堆定时器<br>改用了c++11的线程库，及其同步工具<br>尽可能地把char * 转为std::string<br>更改了网页内容(当然是国漫之光、国漫之巅的不良人是也)<br>使用了json配置文件来存储一些配置项<br>

## 用法
&emsp;配置mysql数据库，需要建立一个用户名和密码的数据库，其中字段为<br>username, passwd，建立完库后，更改.json配置文件，并且一些服务器<br>配置参数也在.json配置文件中，可以根据自己的情况进行更改
&emsp;需要使用jsoncpp库，本项目直接包含了jsoncpp的头文件并和其源文件直接编译的
```shell
git clone git@github.com:nullptroot/myWeb.git
cd myWeb
make
./server configFile.json
```

# other
&emsp;后续会不断的更新迭代，不断增强功能，精简代码。