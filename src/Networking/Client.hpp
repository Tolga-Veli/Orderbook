#pragma once

#include "Networking.hpp"

namespace ob::networking {
class Client {
public:
  Client() = default;

  void Connect();
  void Close();
  bool IsConnected();

  Response SendRequest(const Request &request);

private:
};

} // namespace ob::networking
