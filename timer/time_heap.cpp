#include "time_heap.h"

time_heap::time_heap(int cap):capacity(cap),cur_size(0)
{
        array = new heap_timer *[capacity];
        if(!array)
            throw std::exception();
        for(int i = 0; i < capacity; ++i)
            array[i] = nullptr;
}
time_heap::time_heap(heap_timer **init_array,int size,int capacity):cur_size(size),capacity(capacity)
{
    if(capacity < size)
        throw std::exception();
    array = new heap_timer *[capacity];
    if(array == nullptr)
        throw std::exception();
    for(int i = 0; i < capacity; ++i)
        array[i] = nullptr;
    if(size != 0)
    {
        for(int i = 0; i < size; ++i)
            array[i] = init_array[i];
        for(int i = (cur_size-1)/2; i >= 0; --i)
            percolate_down(i);
    }
}
time_heap::~time_heap()
{
    for(int i = 0; i < cur_size; ++i)
        delete array[i];
    delete [] array;
}
/*heap insert操作*/
void time_heap::add_timer(heap_timer *timer)
{
    if(timer == nullptr)
        return;
    if(cur_size >= capacity)
        resize();
    int hole = cur_size++;
    int parent = 0;
    for(;hole > 0; hole = parent)
    {
        parent = (hole - 1)/2;
        if(array[parent]->expire <= timer->expire)
            break;
        array[hole] = array[parent];
    }
    array[hole] = timer;
}
void time_heap::del_timer(heap_timer *timer)
{
    if(timer == nullptr)
        return;
    timer->cb_func = nullptr;
}
bool time_heap::empty() const
{
    return cur_size == 0;
}
heap_timer * time_heap::top() const
{
    if(empty())
        return nullptr;
    return array[0];
}
void time_heap::pop_timer()
{
    if(empty())
        return;
    if(array[0] != nullptr)
    {
        delete array[0];
        array[0] = array[--cur_size];
        percolate_down(0);
    }
}

void time_heap::tick()
{
    heap_timer *tmp = array[0];
    time_t cur = time(NULL);
    while(!empty())
    {
        if(tmp == nullptr)
            break;
        if(tmp->expire > cur)
            break;
        if(tmp->cb_func != nullptr)
            tmp->cb_func(tmp->user_data);
        pop_timer();
        tmp = array[0];
    }
}

void time_heap::percolate_down(int hole)
{
    heap_timer *temp = array[hole];
    int child = 0;
    for(;(hole * 2 + 1) <= cur_size - 1; hole = child)
    {
        child = hole * 2 + 1;
        if(child < (cur_size-1) && array[child+1]->expire < array[child]->expire)
            ++child;
        if(array[child]->expire < temp->expire)
            array[hole] = array[child];
        else
            break;
    }
    array[hole] = temp;
}
void time_heap::resize()
{
    heap_timer **temp = new heap_timer * [2*capacity];
    if(temp == nullptr)
        throw std::exception();
    for(int i = 0; i < cur_size; ++i)
        temp[i] = nullptr;
    capacity = capacity * 2;
    for(int i = 0; i < cur_size; ++i)
        temp[i] = array[i];
    delete [] array;
    array = temp;
    
}
/*粗暴的加了调整，就是暴力搜索到目标，然后下滤操作*/
 void time_heap::adjust_timer(heap_timer *timer)
 {
    if(timer == nullptr)
        return;
    int hole;
    for(hole = 0; hole < cur_size; ++hole)
    {
        if(array[hole] == timer)
            break;
    }
    percolate_down(hole);
 }