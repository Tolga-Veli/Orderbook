#pragma once

#include <list>
#include <map>
#include <memory_resource>
#include <optional>
#include <unordered_map>

#include "MatchingEngine.hpp"
#include "OrderbookSnapshot.hpp"

namespace ob::engine {
static constexpr std::size_t MEMORY_BUFFER_SIZE = 64 * (1 << 20); // 64 MiB
class Orderbook {
public:
  Orderbook()
      : m_Bids(&m_Resource), m_Asks(&m_Resource), m_Orders(&m_Resource),
        m_MatchingEngine(std::make_unique<FIFO_Matching>()) {}
  ~Orderbook() {}

  Orderbook(const Orderbook &) = delete;
  void operator=(const Orderbook &) = delete;

  Orderbook(Orderbook &&) = delete;
  void operator=(Orderbook &&) = delete;

  [[nodiscard]] OrderID AddOrder(ClientID clientID, Price price,
                                 Quantity quantity, Side side,
                                 OrderType _order_type, TimeInForce tif,
                                 Flags flag);

  void ModifyOrder(OrderID orderID, Price new_price, Quantity new_quantity);
  void CancelOrder(OrderID id);
  void CancelAllOrders();
  void RemoveFillAndKill();

  void SetMatchingStrategy(std::unique_ptr<IMatchingEngine> &engine) {
    m_MatchingEngine = std::move(engine);
  }

  [[nodiscard]] bool HasOrders() const noexcept;
  [[nodiscard]] std::optional<Order> GetOrder(OrderID orderID) const;
  [[nodiscard]] Order *GetBestBid();
  [[nodiscard]] Order *GetBestAsk();
  Quantity GetVolumeAtPrice(Price price) const;
  std::vector<Order> GetOrderByPrice(Price price) const;
  Price GetSpread() const;
  void GetDepth();

  [[nodiscard]] OrderbookSnapshot GetSnapshot(uint32_t depth) const;

  void PrintOrderbook();
  [[nodiscard]] const std::vector<Trade> &GetTradeHistory() const {
    return m_Trades;
  };

private:
  /* struct OrderPointer {
     data::List<Order>::iterator iter;
     data::List<Order> *list_ptr;
   };

   std::shared_ptr<Allocator> m_Pool{nullptr};
   std::map<Price, data::List<Order>, std::greater<Price>> m_Bids;
   std::map<Price, data::List<Order>, std::less<Price>> m_Asks;*/

  struct OrderPointer {
    std::pmr::list<Order>::iterator iter;
    std::pmr::list<Order> *list_ptr;
  };

  std::pmr::monotonic_buffer_resource m_Resource{MEMORY_BUFFER_SIZE};
  std::pmr::map<Price, std::pmr::list<Order>, std::greater<Price>> m_Bids;
  std::pmr::map<Price, std::pmr::list<Order>, std::less<Price>> m_Asks;
  std::pmr::unordered_map<OrderID, OrderPointer> m_Orders;

  std::unique_ptr<IMatchingEngine> m_MatchingEngine;
  std::vector<Trade> m_Trades;

  void MatchIncomingOrders(Order &order);
  void AddOrderInternal(Order &&order);
  void Shutdown() noexcept;
};
} // namespace ob::engine
