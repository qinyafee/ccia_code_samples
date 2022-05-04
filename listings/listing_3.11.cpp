#include <memory>
#include <mutex>

// 一个共享源，构建代价很昂贵
// 使用一个互斥量的延迟初始化(线程安全)过程.
struct some_resource {
  void do_something() {}
};

std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;
void foo() {
  std::unique_lock<std::mutex> lk(
      resource_mutex); //! 1.所有线程在此产生不必要的序列化。
  if (!resource_ptr) {
    resource_ptr.reset(new some_resource); // 只有初始化过程需要保护
  }
  lk.unlock();
  resource_ptr->do_something();
}

// 声名狼藉的双重检查锁(Double-Checked Locking)模式
// 这里有潜在的条件竞争：因为外部的读取锁①没有与内部的写入锁进行同步③。
void undefined_behaviour_with_double_checked_locking() {
  if (!resource_ptr) // 1
                     // 指针第一次读取数据不需要获取锁①，并且只有在指针为NULL时才需要获取锁。
  {
    std::lock_guard<std::mutex> lk(resource_mutex);
    if (!resource_ptr) // 2 当获取锁之后，指针会被再次检查一遍②
                       // (这就是双重检查的部分)，避免另一的线程在第一次检查后再做初始化，并且让当前线程获取锁。
    {
      resource_ptr.reset(new some_resource); // 3
    }
  }
  resource_ptr->do_something(); //! 4 some_resource实例可能没完成创建，结果错误。
}

int main() { foo(); }
