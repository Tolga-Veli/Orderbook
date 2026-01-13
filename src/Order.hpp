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
      : m_OrderID(orderID), m_ClientID(clientID),
        m_Timestamp(core::GetCurrentTime()), m_Price(price),
        m_IntialQuantity(quantity), m_RemainingQuantity(quantity), m_Side(side),
        m_OrderType(order_type), m_TimeInForce(tif), m_Flags(flag) {}
  ~Order() = default;

  const OrderID &GetOrderID() const { return m_OrderID; }
  const ClientID &GetClientID() const { return m_ClientID; }
  Price GetPrice() const { return m_Price; }
  Quantity GetInitialQuantity() const { return m_IntialQuantity; }
  Quantity GetRemainingQuantity() const { return m_RemainingQuantity; }
  Quantity GetFilledQuantity() const {
    return m_IntialQuantity - m_RemainingQuantity;
  }
  Time GetTime() const { return m_Timestamp; }
  Side GetSide() const { return m_Side; }
  OrderType GetOrderType() const { return m_OrderType; }
  TimeInForce GetTimeInForce() const { return m_TimeInForce; }
  Flags GetFlags() const { return m_Flags; }
  bool isFilled() const { return m_RemainingQuantity == 0; }

  void ModifyOrder(Price newm_Price, Quantity new_quantity) {
    Quantity filled = GetFilledQuantity();
    if (filled > new_quantity)
      throw std::logic_error(
          "Modified quantity is higher than the already filled quantity.");

    m_Price = newm_Price;
    m_RemainingQuantity = new_quantity - filled;
    m_Timestamp = core::GetCurrentTime();
  }

  void Fill(Quantity quantity) {
    if (quantity > m_RemainingQuantity)
      throw std::logic_error(std::format(
          "Order ({}) cannot be filled for more than its capcity.", m_OrderID));

    m_RemainingQuantity -= quantity;
  }

  void info() const {
    std::cout << "OrderID: " << m_OrderID << " , ClientID: " << m_ClientID
              << " , Price: " << m_Price
              << " , Initial Quantity: " << m_IntialQuantity
              << " , Remaining Quantity: " << m_RemainingQuantity
              << " , Time: " << m_Timestamp
              << " , Side: " << core::to_string(m_Side)
              << " , Order Type: " << core::to_string(m_OrderType)
              << " , Time in Force: " << core::to_string(m_TimeInForce)
              << " , Flags: " << core::to_string(m_Flags) << '\n';
  }

private:
  const OrderID m_OrderID;
  const ClientID m_ClientID;
  Price m_Price;
  Quantity m_IntialQuantity, m_RemainingQuantity;
  Time m_Timestamp;
  Side m_Side;
  OrderType m_OrderType;
  TimeInForce m_TimeInForce;
  Flags m_Flags;
};
} // namespace engine
} // namespace ob