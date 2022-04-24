#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>

std::vector<int> data;
std::atomic_bool data_ready(false); // 必须用返回值初始化，不能用返回引用初始化

void reader_thread()
{
    while(!data_ready.load()) //显示的调用load()，是一个加载操作，返回存储的值
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout<<"The answer="<<data[0]<<"\n";
}
void writer_thread()
{
    data.push_back(42);
    data_ready=true; //
}
