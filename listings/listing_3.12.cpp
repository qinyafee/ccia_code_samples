// 使用 std::call_once 作为类成员的延迟初始化(线程安全)

#include <mutex>

struct connection_info
{};

struct data_packet
{};

struct connection_handle
{
    void send_data(data_packet const&)
    {}
    data_packet receive_data()
    {
        return data_packet();
    }
};

struct remote_connection_manager
{
    connection_handle open(connection_info const&)
    {
        return connection_handle();
    }
} connection_manager;


class X
{
private:
    connection_info connection_details;
    connection_handle connection;
    std::once_flag connection_init_flag;

    void open_connection()
    {
        connection=connection_manager.open(connection_details);
    }
public:
    X(connection_info const& connection_details_):
        connection_details(connection_details_)
    {}

    // 第一个调用send_data()①或receive_data()③的线程完成初始化过程。
    void send_data(data_packet const& data) //1
    {
        std::call_once(connection_init_flag,&X::open_connection,this); //2
        connection.send_data(data);
    }
    data_packet receive_data() //3
    {
        std::call_once(connection_init_flag,&X::open_connection,this); //4
        return connection.receive_data();
    }
};

int main()
{}
