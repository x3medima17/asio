#include <gtest/gtest.h>

#include "Client.hpp"
#include "Server.hpp"

TEST(ServerTest, BasicTest) {
  std::vector<std::vector<Message>> msgs(2);
  Chunk chunk;

  for (size_t i = 0; i < CHUNK_SIZE; i++)
    for (size_t j = 0; j < FRAGMENT_SIZE; j++) {
      auto val = i * FRAGMENT_SIZE + j;
      chunk[i][j] = val;
      msgs[val % 2].emplace_back(i, j, val);
    }

  auto server = Server("127.0.0.1", 1234);
  std::thread server_thread([&server]() { server.run_server(); });

  std::vector<std::thread> clients_threads(2);

  for (size_t i = 0; i < clients_threads.size(); i++)
    clients_threads[i] = std::thread([&msgs, i]() {
      Client curr("127.0.0.1", 1234, "lo0");
      curr.feedServer(msgs[i]);
    });

  for (auto &client : clients_threads) {
    client.join();
  }

  server_thread.join();
  server.wait();

  EXPECT_EQ(chunk, server.getChunk());
}