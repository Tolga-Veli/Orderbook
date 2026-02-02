#pragma once

#include "Commands.hpp"
#include "globals.hpp"

#include <variant>

namespace ob::networking {
using RequestID = std::uint64_t;

struct Request {
  RequestID requestID;
  engine::Command payload;
};

enum class ErrorCode : std::uint8_t {
  None = 0,
  InvalidRequest = 1,
};

struct Ok {
  OrderID orderID;
};

struct Error {
  ErrorCode errorCode;
  std::string msg;
};

struct Response {
  RequestID requestID;
  std::variant<Ok, Error> payload;
};

}; // namespace ob::networking
//
