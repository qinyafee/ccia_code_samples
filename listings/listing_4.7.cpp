#include <future>
#include <string>

struct X {
  void foo(int, std::string const &);
  std::string bar(std::string const &);
};

X x;
// 调用p->foo(42, "hello")，p是指向x的指针
auto f1 = std::async(&X::foo, &x, 42, "hello");
// 调用tmpx.bar("goodbye")， tmpx是x的拷贝副本
auto f2 = std::async(&X::bar, x, "goodbye");

struct Y {
  double operator()(double);
};
Y y;
// 调用tmpy(3.141)，tmpy通过Y的移动构造函数得到
auto f3 = std::async(Y(), 3.141);
auto f4 = std::async(std::ref(y), 2.718); // 调用y(2.718)

X baz(X &);
auto f6 = std::async(baz, std::ref(x)); // 调用baz(x)
class move_only {
public:
  move_only();
  move_only(move_only &&);
  move_only(move_only const &) = delete;
  move_only &operator=(move_only &&);
  move_only &operator=(move_only const &) = delete;
  void operator()();
};
// 调用tmp()，tmp是通过std::move(move_only())构造得到
auto f5 = std::async(move_only());

//
auto f6 = std::async(std::launch::async, Y(), 1.2); // 在新线程上执行

// 延迟到在wait()或get()调用时执行
auto f7 = std::async(std::launch::deferred, baz, std::ref(x));

// 实现可以选择这两种方式的一种。最后一个选项是默认的。
auto f8 =
    std::async(std::launch::deferred | std::launch::async, baz, std::ref(x));

auto f9 = std::async(baz, std::ref(x));

f7.wait(); // 调用延迟函数