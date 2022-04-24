#include <map>
#include <string>
#include <mutex>
#include <shared_mutex> // (since C++17)，之前可以用boost的

class dns_entry
{};

class dns_cache
{
    std::map<std::string,dns_entry> entries;
    std::shared_mutex entry_mutex;
public:
    dns_entry find_entry(std::string const& domain)
    {
        std::shared_lock<std::shared_mutex> lk(entry_mutex); //获取共享访问权，保护其共享和只读权限①
        std::map<std::string,dns_entry>::const_iterator const it=
            entries.find(domain);
        return (it==entries.end())?dns_entry():it->second;
    }
    void update_or_add_entry(std::string const& domain,
                             dns_entry const& dns_details)
    {
        std::lock_guard<std::shared_mutex> lk(entry_mutex); // 更新线程获取独占访问。
        entries[domain]=dns_details;
    }
};

int main()
{}
