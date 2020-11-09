#include "Client.hpp"


Client::Client(const std::string &server_ip,
               uint16_t server_port,
               const std::string &local_interface)  {
  sock = std::make_unique<tcp::socket>(io_service);
  sock->open(boost::asio::ip::tcp::v4());

  setInterface(local_interface);

  auto remote_ip = boost::asio::ip::address::from_string(server_ip);
  auto endpoint = tcp::endpoint(remote_ip, server_port);
  sock->connect(endpoint);
}

void Client::feedServer(const std::vector<Message> &msgs)  {
  for (const auto &msg : msgs) {
    std::string json = msg.toJson();
    boost::system::error_code ec;
    boost::asio::write(*sock, boost::asio::buffer(json), ec);
  }
}

void Client::disconnect() {
  sock->close();
}

void Client::setInterface(const std::string &local_interface)  {
  auto native_sock = sock->native_handle();
  auto index = if_nametoindex(local_interface.c_str());
  if (index == 0 && errno == ENXIO) {
    throw std::runtime_error("Interface not found");
  }
  auto suc =
      setsockopt(native_sock, IPPROTO_IP, IP_BOUND_IF, &index, sizeof(index));
  if (suc != 0) {
    throw std::runtime_error("Failed to bind socket to interface");
  }
}
