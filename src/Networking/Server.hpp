#pragma once

#include "Networking.hpp"
#include "Orderbook.hpp"

namespace ob::networking {
class Server {
public:
  Server() = default;

  void Close();
  Response handle(const Request &request);

private:
  // Orderbook lives during the lifetime of the server
  engine::Orderbook m_Orderbook;
};
}; // namespace ob::networking
//
