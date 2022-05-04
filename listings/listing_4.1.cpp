// 当一个线程等待另一个线程完成任务时
// 第一，它可以持续的检查共享数据标志(用于做保护工作的互斥量)，直到另一线程完成工作时对这个标志进行重设。
// 是一种浪费：线程消耗宝贵的执行时间持续的检查对应标志，并且当互斥量被等待线程上锁
// 后，其他线程就没有办法获取锁

//第二，是在等待线程在检查间隙，使用 std::this_thread::sleep_for()
//进行周期性的间歇
//这个实现就进步很多，因为当线程休眠时，线程没有浪费执行时间，但是很难确定正确的休眠时间。
//太短的休眠和没有休眠一样，都会浪费执行时间；太长的休眠时间，可能会让任务等待线程醒来。
bool flag;
std::mutex m;
void wait_for_flag() {
  std::unique_lock<std::mutex> lk(m);
  while (!flag) {
    lk.unlock(); // 1 解锁互斥量
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 2 休眠100ms
    lk.lock(); // 3 再锁互斥量
  }
}

/******************/
//第三，
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

bool more_data_to_prepare() { return false; }

struct data_chunk {};

data_chunk prepare_data() { return data_chunk(); }

void process(data_chunk &) {}

bool is_last_chunk(data_chunk &) { return true; }

std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond; //仅限于与 std::mutex 一起工作， 首选

void data_preparation_thread() {
  while (more_data_to_prepare()) {
    data_chunk const data = prepare_data();
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(data);
    data_cond.notify_one(); // 对等待的线程(如果有等待线程)进行通知③。
  }
}

void data_processing_thread() {
  while (true) {
    std::unique_lock<std::mutex> lk(
        mut); // 此处必须用std::unique_lock而不是std::lock_guard，上锁/解锁需要灵活操作

    // 收到notify_one()的通知，获取互斥锁，并且wait()函数对条件lambda表达式再次检查。
    // 在条件满足的情况下，从wait()返回并继续持有锁。
    // 当条件不满足时，线程将对互斥量解锁，并且重新开始等待。
    data_cond.wait(lk, [] { return !data_queue.empty(); });
    data_chunk data = data_queue.front();
    data_queue.pop();
    lk.unlock();
    process(data);
    if (is_last_chunk(data))
      break;
  }
}

int main() {
  std::thread t1(data_preparation_thread);
  std::thread t2(data_processing_thread);

  t1.join();
  t2.join();
}
