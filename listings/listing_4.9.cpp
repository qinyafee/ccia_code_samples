#include <deque>
#include <future>
#include <mutex>
#include <thread>
#include <utility>

std::mutex m;
std::deque<std::packaged_task<void()>> tasks;

bool gui_shutdown_message_received();
void get_and_process_gui_message();

void gui_thread() {
  while (!gui_shutdown_message_received()) {
    get_and_process_gui_message(); //进行轮询界面消息处理
    std::packaged_task<void()> task;
    {
      std::lock_guard<std::mutex> lk(m);
      if (tasks.empty()) // 4 当队列中没有任务，它将再次循环；
        continue;
      task = std::move(
          tasks.front()); // 5 在队列中提取出一个任务，然后释放队列上的锁
      tasks.pop_front();
    }
    task(); // 6执行任务
  }
}

std::thread gui_bg_thread(gui_thread);

template <typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
  std::packaged_task<void()> task(f); // 7 可以提供一个打包好的任务
  std::future<void> res = task.get_future();
  std::lock_guard<std::mutex> lk(m);
  tasks.push_back(std::move(task));
  return res;
}
