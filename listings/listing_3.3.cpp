#include <cstddef>
#include <deque>
template <typename T, typename Container = std::deque<T>> class stack {
public:
  explicit stack(const Container &);
  explicit stack(Container && = Container());
  template <class Alloc> explicit stack(const Alloc &);
  template <class Alloc> stack(const Container &, const Alloc &);
  template <class Alloc> stack(Container &&, const Alloc &);
  template <class Alloc> stack(stack &&, const Alloc &);

  bool empty() const;
  size_t size() const;
  T &top();
  T const &top() const;
  void push(T const &);
  void push(T &&);
  void pop();
  void swap(stack &&);
};

int main() {
  stack<int> s;
  // 单线程安全代码，多线程不安全
  // 因为在调用empty()①和调用top()②之间，可能有来自另一个线程的pop()调用并删除了最后一个元素
	// 另一个潜在的条件竞争在调用top()②和pop()③之间
  if (!s.empty()) {            // 1
    int const value = s.top(); // 2
    s.pop();                   // 3
    // do_something(value);
  }
}
