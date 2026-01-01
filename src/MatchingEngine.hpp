#pragma once

#include <map>
#include "globals.hpp"
#include "Order.hpp"
#include "Trade.hpp"

namespace ob {
namespace engine {

class Orderbook;

class IMatchingEngine {
public:
  virtual ~IMatchingEngine() = default;
  virtual std::vector<Trade> Match(Order &order, Orderbook &book) = 0;
};

class FIFO_Matching : public IMatchingEngine {
public:
  std::vector<Trade> Match(Order &order, Orderbook &book) override;

private:
};
} // namespace engine
} // namespace ob