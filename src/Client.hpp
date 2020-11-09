#pragma once

#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

#include "Message.hpp"
#include "chunk.hpp"

using boost::asio::buffers_begin;
using boost::asio::ip::tcp;


class Client {
 public:
  Client(const std::string &server_ip,
         uint16_t server_port,
         const std::string &local_interface);

  void feedServer(const std::vector<Message>& msgs);

  void disconnect();

 private:
  void setInterface(const std::string &local_interface);

  boost::asio::io_service io_service;
  std::unique_ptr<tcp::socket> sock;
};
