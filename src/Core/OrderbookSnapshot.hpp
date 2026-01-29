#pragma once

#include <vector>
#include "globals.hpp"
#include "Trade.hpp"

namespace ob::engine {
struct OrderbookSnapshot {
  std::vector<std::pair<Price, Quantity>> bids, asks;
  std::vector<Trade> trades;
};

}; // namespace ob::engine