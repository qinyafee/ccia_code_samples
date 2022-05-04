#include <future>
// 使用“承诺”解决单线程多连接问题

class connection_set {};

void process_connections(connection_set &connections) {
  while (!done(connections)) {
    for (connection_iterator connection = connections.begin(),
                             end = connections.end();
         connection != end; ++connection) {
      if (connection->has_incoming_data()) {
        data_packet data = connection->incoming(); // 3检索是否有数据
        std::promise<payload_type> &p = connection->get_promise(data.id);
        p.set_value(data.payload); //
      }
      if (connection->has_outgoing_data()) { // 5检索是否有正在发送已入队的传出数据
        outgoing_packet data = connection->top_of_outgoing_queue();
        connection->send(data.payload);
        data.promise.set_value(true); //6当发送完成，与传出数据相关的“承诺”将置为true
      }
    }
  }
}

//待续
//4.2.4 为“期望”存储“异常”