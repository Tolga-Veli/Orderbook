#pragma once

#include <map>
#include <unordered_map>
#include <optional>

#include "List.tpp"
#include "Order.hpp"
#include "Trade.hpp"
#include "MatchingEngine.hpp"

namespace ob {
namespace engine {

class Orderbook {
public:
  void AddOrder(ClientID clientID, Price price, Quantity quantity, Side side,
                OrderType _order_type, TimeInForce tif, Flags flag);

  void ModifyOrder(OrderID orderID, Price new_price, Quantity new_quantity);
  void CancelOrder(OrderID id);
  void CancelAllOrders();
  void RemoveFillAndKill();

  void SetMatchingStrategy(std::unique_ptr<IMatchingEngine> &engine) {
    matchingEngine_ = std::move(engine);
  }

  [[nodiscard]] bool HasOrders() const;
  [[nodiscard]] std::optional<std::reference_wrapper<Order>>
  GetOrder(OrderID orderID) const;

  Order *GetBestBid() const;
  Order *GetBestAsk() const;
  Quantity GetVolumeAtPrice(Price price) const;
  std::vector<Order> GetOrderByPrice(Price price) const;
  Price GetSpread() const;
  void GetDepth();

  void PrintOrderbook();
  const std::vector<Trade> &GetTradeHistory() const { return trades_; };

  Orderbook() : matchingEngine_(std::make_unique<FIFO_Matching>()) {}
  Orderbook(const Orderbook &) = delete;
  Orderbook(Orderbook &&) = delete;
  void operator=(const Orderbook &) = delete;
  void operator=(Orderbook &&) = delete;
  ~Orderbook() { Shutdown(); }

private:
  struct OrderPointer {
    data::List<Order>::iterator iter;
    data::List<Order> *list_ptr;
  };

  std::map<Price, data::List<Order>, std::greater<Price>> bids_;
  std::map<Price, data::List<Order>, std::less<Price>> asks_;
  std::unordered_map<OrderID, OrderPointer> orders_;
  std::vector<Trade> trades_;

  std::unique_ptr<IMatchingEngine> matchingEngine_;

  void MatchIncomingOrders(Order &order);
  void AddOrderInternal(Order &&order);
  void Shutdown();
};
} // namespace engine
} // namespace ob