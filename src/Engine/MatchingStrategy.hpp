#pragma once

#include "Order.hpp"
#include "Trade.hpp"

namespace ob::engine {

class OrderBook;

class IMatchingStrategy {
public:
  virtual ~IMatchingStrategy() = default;
  virtual std::vector<Trade> Match(Order &order, OrderBook &book) = 0;
  static uint32_t m_Counter;
};

class FIFO_Matching : public IMatchingStrategy {
public:
  std::vector<Trade> Match(Order &order, OrderBook &book) override;
};
} // namespace ob::engine
