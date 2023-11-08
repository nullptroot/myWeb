
filterSrc = ./http/test.cpp ./log/test.cpp ./timer/time_heap.cpp ./timer/timeslice.cpp
libNeed = -lmysqlclient
stdNeed = -std=c++20

allSrc=$(wildcard ./*.cpp ./CGImysql/*.cpp ./http/*.cpp ./log/*.cpp ./timer/*.cpp)
# $(info $(allSrc))
src=$(filter-out $(filterSrc),$(allSrc))
# $(info $(src))
objs=$(patsubst %.cpp, %.o, $(src))
# $(info $(objs))
target = server
$(target) : $(objs)
	g++ -o $(target) $(objs) $(libNeed) $(stdNeed)

%.o : %.cpp
	g++ -c $< -o $@ $(libNeed) $(stdNeed)


.PHONY:clean
clean:
	rm $(objs) -f