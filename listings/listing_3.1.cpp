#include <algorithm>
#include <list>
#include <mutex>

// 当访问共享数据前，使用互斥量将相关数据锁住，再当访问结束后，再将数据解锁。
std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value) {
	// RAII语法的模板类 std::lock_guard
  std::lock_guard<std::mutex> guard(some_mutex);
  some_list.push_back(new_value);
}
bool list_contains(int value_to_find) {
  std::lock_guard<std::mutex> guard(some_mutex);
  return std::find(some_list.begin(), some_list.end(), value_to_find) !=
         some_list.end();
}

#include <iostream>

int main() {
  add_to_list(42);
  std::cout << "contains(1)=" << list_contains(1)
            << ", contains(42)=" << list_contains(42) << std::endl;
}
