
#include "Server.hpp"

Server::Server(const std::string &interface, uint16_t port) {
  auto endpoint =
      tcp::endpoint(boost::asio::ip::address::from_string(interface), port);
  acceptor = std::make_unique<tcp::acceptor>(io_service, endpoint);
  workers = std::vector<std::thread>(2);
  chunk.fill({});
}

void Server::run_server() {
  for (auto &worker : workers) {
    auto socket = std::make_shared<tcp::socket>(io_service);
    acceptor->accept(*socket);
    worker = std::thread([this, socket]() { this->handle_socket(socket); });
  }
}

auto Server::getChunk() const -> Chunk {
  return chunk;
}

Server::~Server() {
  wait();
  //  print_chunk(chunk);
}

void Server::handle_socket(std::shared_ptr<tcp::socket> socket) {
  boost::asio::streambuf buf;
  while (auto msg = read_message(*socket, buf)) {
    auto message = *msg;
    chunk.at(message.fragment_id).at(message.offset) = message.val;
  }
}

auto Server::read_message(tcp::socket &socket, boost::asio::streambuf &buf)
    -> std::optional<Message> {
  boost::system::error_code ec;
  boost::asio::read_until(socket, buf, '\n', ec);
  if (ec) {
    if (ec != boost::asio::error::eof)
      BOOST_LOG_TRIVIAL(warning)
          << "Unhandled read_until() ec: " << ec.message();

    return std::nullopt;
  }

  std::istream is(&buf);
  std::string result_line;
  std::getline(is, result_line);
  return Message(result_line);
}

void Server::wait() {
  if (finished) return;
  for (auto &worker : workers) {
    worker.join();
  }
  finished = true;
}