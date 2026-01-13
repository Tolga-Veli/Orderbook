#pragma once

#include <map>
#include <unordered_map>
#include <optional>

#include "List.tpp"
#include "OrderbookSnapshot.hpp"
#include "MatchingEngine.hpp"

namespace ob::engine {
class Orderbook {
public:
  Orderbook() : m_MatchingEngine(std::make_unique<FIFO_Matching>()) {}
  ~Orderbook() { Shutdown(); }
  Orderbook(const Orderbook &) = delete;
  void operator=(const Orderbook &) = delete;
  Orderbook(Orderbook &&) = delete;
  void operator=(Orderbook &&) = delete;

  OrderID AddOrder(ClientID clientID, Price price, Quantity quantity, Side side,
                   OrderType _order_type, TimeInForce tif, Flags flag);

  void ModifyOrder(OrderID orderID, Price new_price, Quantity new_quantity);
  void CancelOrder(OrderID id);
  void CancelAllOrders();
  void RemoveFillAndKill();

  void SetMatchingStrategy(std::unique_ptr<IMatchingEngine> &engine) {
    m_MatchingEngine = std::move(engine);
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

  OrderbookSnapshot GetSnapshot(uint32_t depth) const;

  void PrintOrderbook();
  const std::vector<Trade> &GetTradeHistory() const { return m_Trades; };

private:
  struct OrderPointer {
    data::List<Order>::iterator iter;
    data::List<Order> *list_ptr;
  };

  std::map<Price, data::List<Order>, std::greater<Price>> m_Bids;
  std::map<Price, data::List<Order>, std::less<Price>> m_Asks;
  std::unordered_map<OrderID, OrderPointer> m_Orders;

  std::unique_ptr<IMatchingEngine> m_MatchingEngine;
  std::vector<Trade> m_Trades;

  void MatchIncomingOrders(Order &order);
  void AddOrderInternal(Order &&order);
  void Shutdown();
};
} // namespace ob::engine