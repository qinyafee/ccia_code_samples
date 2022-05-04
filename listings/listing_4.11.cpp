#include <chrono>
#include <condition_variable>
#include <mutex>

// 等待一个条件变量——有超时功能

std::condition_variable cv;
bool done;
std::mutex m;

bool wait_loop() {
  auto const timeout =
      std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
  std::unique_lock<std::mutex> lk(m);
  while (!done) {
    if (cv.wait_until(lk, timeout) == std::cv_status::timeout) //
      break;
  }
  return done;
}
