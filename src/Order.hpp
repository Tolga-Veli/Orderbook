#pragma once

#include "globals.hpp"
#include <iostream>

namespace ob {
namespace engine {
class Order {
public:
  Order() = delete;
  Order(OrderID orderID, ClientID clientID, Price price, Quantity quantity,
        Side side, OrderType order_type, TimeInForce tif, Flags flag)
      : orderID_(orderID), clientID_(clientID), timestamp_(GetCurrentTime()),
        price_(price), initialQuantity_(quantity), remainingQuantity_(quantity),
        side_(side), orderType_(order_type), tif_(tif), flags_(flag) {}
  ~Order() = default;

  const OrderID &GetOrderID() const { return orderID_; }
  const ClientID &GetClientID() const { return clientID_; }
  Price GetPrice() const { return price_; }
  Quantity GetInitialQuantity() const { return initialQuantity_; }
  Quantity GetRemainingQuantity() const { return remainingQuantity_; }
  Quantity GetFilledQuantity() const {
    return initialQuantity_ - remainingQuantity_;
  }
  Time GetTime() const { return timestamp_; }
  Side GetSide() const { return side_; }
  OrderType GetOrderType() const { return orderType_; }
  TimeInForce GetTimeInForce() const { return tif_; }
  Flags GetFlags() const { return flags_; }
  bool isFilled() const { return remainingQuantity_ == 0; }

  void ModifyOrder(Price newprice_, Quantity new_quantity) {
    Quantity filled = GetFilledQuantity();
    if (filled > new_quantity)
      throw std::logic_error(
          "Modified quantity is higher than the already filled quantity.");

    price_ = newprice_;
    remainingQuantity_ = new_quantity - filled;
    timestamp_ = GetCurrentTime();
  }

  void Fill(Quantity quantity) {
    if (quantity > remainingQuantity_)
      throw std::logic_error(std::format(
          "Order ({}) cannot be filled for more than its capcity.", orderID_));

    remainingQuantity_ -= quantity;
  }

  void info() const {
    std::cout << "OrderID: " << orderID_ << " , ClientID: " << clientID_
              << " , Price: " << price_
              << " , Initial Quantity: " << initialQuantity_
              << " , Remaining Quantity: " << remainingQuantity_
              << " , Time: " << timestamp_ << " , Side: " << to_string(side_)
              << " , Order Type: " << to_string(orderType_)
              << " , Time in Force: " << to_string(tif_)
              << " , Flags: " << to_string(flags_) << '\n';
  }

private:
  const OrderID orderID_;
  const ClientID clientID_;
  Price price_;
  Quantity initialQuantity_, remainingQuantity_;
  Time timestamp_;
  Side side_;
  OrderType orderType_;
  TimeInForce tif_;
  Flags flags_;
};
} // namespace engine
} // namespace ob