#include <atomic>
class spinlock_mutex //自旋互斥锁。可以给std::lock_guard<>使用
{
    std::atomic_flag flag;
public:
    spinlock_mutex():
        flag(ATOMIC_FLAG_INIT) //std::atomic_flag 类型的对象必须被ATOMIC_FLAG_INIT初始化。“清除”状态
    {}
    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire));//设置，②使用默认内存顺序设置
    }
    void unlock()
    {
        flag.clear(std::memory_order_release); //销毁，使用释放语义清除标志
    }
};
