#pragma once

#include "globals.hpp"

#include <variant>

namespace ob::networking {
using RequestID = std::uint64_t;

enum class RequestType : std::uint8_t {
  ADD = 1,
  MODIFY,
  CANCEL,
};

struct AddOrder {
  ClientID clientID;
  Price price;
  Quantity quantity;
  Side side;
  OrderType order_type;
  TimeInForce tif;
  Flags flags;
};

struct ModifyOrder {
  OrderID orderID;
  Price new_price;
  Quantity new_quantity;
};

struct CancelOrder {
  OrderID orderID;
};

struct Request {
  RequestID requestID;
  RequestType request_type;
  std::variant<AddOrder, ModifyOrder, CancelOrder> payload;
};

enum class ErrorCode : std::uint8_t {
  InvalidRequest,
  UnknownOrder,
  PermissionDenied,
  EngineRejected,
};

struct Ok {
  std::optional<OrderID> orderID;
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
