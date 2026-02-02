#pragma once

#include "CommandQueue.hpp"
#include "Networking.hpp"

namespace ob::networking {
class Server {
public:
  Server() : m_Orderbook(), m_Queue(m_Orderbook) {}

  void Close();
  Response handle(const Request &request);

private:
  engine::OrderBook m_Orderbook;
  engine::CommandQueue m_Queue;
};
}; // namespace ob::networking
//
