#include <thread>

void some_function()
{}

void some_other_function(int)
{}

std::thread f()
{
    void some_function();
    return std::thread(some_function);
}
std::thread g()
{
    void some_other_function(int);
    std::thread t(some_other_function,42);
    return t;
}

int main()
{
    // std::thread t1=f();
    // t1.join();
    // std::thread t2=g();
    // t2.join();

    std::thread t1(some_function); // 1
    std::thread t2=std::move(t1); // 2
    t1=std::thread(some_other_function, 6); // 3
    std::thread t3; // 4
    t3=std::move(t2); // 5
    t1=std::move(t3); // 6  赋值操作将使程序崩溃，不能通过赋一个新值给 std::thread 对象的方式来"丢弃"一个线程
}
