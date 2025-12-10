#pragma once

#include <map>
#include <optional>

#include "LinkedList.tpp"
#include "Order.hpp"
#include "Trade.hpp"

namespace ob {
namespace engine {
using core::ClientID;
using core::Flags;
using core::Order;
using core::OrderID;
using core::OrderType;
using core::Price;
using core::Quantity;
using core::Side;
using core::Time;
using core::TimeInForce;
using core::Trade;

class Orderbook {
public:
  void AddOrder(ClientID clientID, Price price, Quantity quantity, Side side,
                OrderType _order_type, TimeInForce tif, Flags flag);

  std::optional<Order> GetOrder(OrderID orderID);
  void ModifyOrder(const OrderID &orderID, Price new_price,
                   Quantity new_quantity);
  void KillOrder(const OrderID &id);
  void CancelAllOrders();
  void MatchOrders();

  Order GetBestBid();
  Order GetBestAsk();
  std::vector<Order> GetOrderByPrice(Price price);
  std::vector<Trade> GetTradeHistory();
  Quantity GetVolumeAtPrice(Price price);
  Price GetSpread();
  void GetDepth();

  void PrintOrderbook();
  void Shutdown();

  Orderbook() {}
  ~Orderbook() { Shutdown(); }

private:
  std::map<Price, data::LinkedList<Order>, std::greater<Price>> bids_;
  std::map<Price, data::LinkedList<Order>, std::less<Price>> asks_;
  std::map<OrderID, data::LinkedList<Order>::node *> orders_;
  Time _closingTime;

  void AddOrderInternal(Order order);
};
} // namespace engine
} // namespace ob