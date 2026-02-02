#pragma once

#include <list>
#include <map>
#include <memory_resource>
#include <optional>
#include <unordered_map>
#include <variant>

#include "Commands.hpp"
#include "MatchingStrategy.hpp"
#include "OrderBookSnapshot.hpp"

namespace ob::engine {
static constexpr std::size_t MEMORY_BUFFER_SIZE = 64 * (1 << 20); // 64 MiB
class OrderBook {
public:
  explicit OrderBook(std::unique_ptr<IMatchingStrategy> strategy =
                         std::make_unique<FIFO_Matching>())
      : m_Bids(&m_Resource), m_Asks(&m_Resource), m_Orders(&m_Resource),
        m_MatchingEngine(std::move(strategy)) {}
  ~OrderBook() {}

  OrderBook(const OrderBook &) = delete;
  OrderBook &operator=(const OrderBook &) = delete;

  OrderBook(OrderBook &&) = delete;
  OrderBook &operator=(OrderBook &&) = delete;

  void Apply(const Command &cmd) {
    std::visit([this](auto &&c) { Handle(c); }, cmd);
  }

  void Shutdown() noexcept { m_Running = false; };

  bool HasOrders() const noexcept;
  std::optional<Order> GetOrder(OrderID orderID) const;

  Order *GetBestBid();
  Order *GetBestAsk();
  Quantity GetBidVolumeAtPrice(Price price) const;
  Quantity GetAskVolumeAtPrice(Price price) const;

  OrderBookSnapshot GetSnapshot(uint32_t depth) const;

  const std::vector<Trade> &GetTradeHistory() const { return m_Trades; };

  std::size_t GetBidsDepth() const { return m_Bids.size(); }
  std::size_t GetAsksDepth() const { return m_Asks.size(); }

  void PrintOrderBook() const;

private:
  struct OrderPointer {
    std::pmr::list<Order>::iterator iter;
    std::pmr::list<Order> *list_ptr;
  };

  bool m_Running = true;
  std::pmr::monotonic_buffer_resource m_Resource{MEMORY_BUFFER_SIZE};
  std::pmr::map<Price, std::pmr::list<Order>, std::greater<Price>> m_Bids;
  std::pmr::map<Price, std::pmr::list<Order>, std::less<Price>> m_Asks;
  std::pmr::unordered_map<OrderID, OrderPointer> m_Orders;
  std::unique_ptr<IMatchingStrategy> m_MatchingEngine;
  std::vector<Trade> m_Trades;

  // Handle the types of commands from the command queue
  void Handle(const std::monostate &empty) {}
  void Handle(const cmd::AddOrder &add);
  void Handle(const cmd::ModifyOrder &modify);
  void Handle(const cmd::CancelOrder &cancel);

  void MatchIncomingOrders(Order &order);
};
} // namespace ob::engine
