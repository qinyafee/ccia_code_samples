#include <future>
#include <iostream>

// 需要长时间的运算，任务的结果你不着急
int find_the_answer_to_ltuae() { return 42; }

void do_other_stuff() {}

int main() {

  // std::thread 并不提供直接接收返回值的机制, std::async可以
  std::future<int> the_answer = //返回一个 std::future对象，持有计算结果。
      std::async(find_the_answer_to_ltuae); // async 启动一个异步任务。

  do_other_stuff();

  std::cout << "The answer is " << the_answer.get() << std::endl; // 调用get()；
}

// std::future<>：唯一期望(unique futures)，实例只能与一个指定事件相关联。
// 是只移动的

// std::shared_future<>：共享期望(shared futures)，实例就能关联多个事件，所有实例会在同时变为就绪状态。
// std::shared_future 实例是可拷贝的，所以多个对象可以引用同一关联“期望”的结果。