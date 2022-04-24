#include <mutex>

class some_data
{
    int a;
    std::string b;
public:
    void do_something()
    {}
};

class data_wrapper
{
private:
    some_data data;
    std::mutex m;
public:
    template<typename Function>
    void process_data(Function func)
    {
        std::lock_guard<std::mutex> l(m);
        func(data);
    }
};

some_data* unprotected;

void malicious_function(some_data& protected_data) // 恶意
{
    unprotected=&protected_data; //切勿将受保护数据的指针或引用传递到互斥锁作用域之外
}

data_wrapper x;

void foo()
{
    x.process_data(malicious_function);  // 2 传递一个恶意函数
    unprotected->do_something(); // 3 在无保护的情况下访问保护数据
}

int main()
{
    foo();
}
