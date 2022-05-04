#include <string>
#include <vector>
#include <future>

// std::packaged_task对象是一个可调用对象，并且它可以包含在一个std::function对象中
template<>
class packaged_task<std::string(std::vector<char>*,int)>
{
public:
    template<typename Callable>
    explicit packaged_task(Callable&& f);

    std::future<std::string> get_future();
    void operator()(std::vector<char>*,int);
};
