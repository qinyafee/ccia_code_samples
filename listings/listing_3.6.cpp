#include <mutex>

class some_big_object
{};

//避免死锁的一般建议，就是让两个互斥量总以相同的顺序上锁：总在互斥量B之前锁住互斥量A，
//但是在参数交换了之后，程序又死锁了！
void swap(some_big_object& lhs,some_big_object& rhs)
{}

class X
{
private:
    some_big_object some_detail;
    mutable std::mutex m;
public:
    X(some_big_object const& sd):some_detail(sd){}

    friend void swap(X& lhs, X& rhs)
    {
        if(&lhs==&rhs) //
            return;
        std::lock(lhs.m,rhs.m); // std::lock ——可以一次性锁住多个(两个以上)的互斥量，并且没有副作用(死锁风险)
        // ，所以 std::lock 要么将两个锁都锁住，要不一个都不锁
        std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock);
        swap(lhs.some_detail,rhs.some_detail);
    }
};

int main()
{}
