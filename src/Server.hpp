#pragma once

#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

#include "Message.hpp"
#include "chunk.hpp"

using boost::asio::buffers_begin;
using boost::asio::ip::tcp;


class Server final {
 public:
  Server(const std::string &interface, uint16_t port);

  void run_server();

  auto getChunk() const -> Chunk;

  void wait();

  ~Server();

 private:
  void handle_socket(std::shared_ptr<tcp::socket> socket);

  static auto read_message(tcp::socket &socket, boost::asio::streambuf &buf)
  -> std::optional<Message>;

  boost::asio::io_service io_service;
  std::unique_ptr<tcp::acceptor> acceptor;
  std::vector<std::thread> workers;
  bool finished{false};
  Chunk chunk{};
};