#pragma once

#include "globals.hpp"

#include <iostream>

namespace ob {
namespace engine {
class Trade {
public:
  Trade() = delete;
  Trade(TradeID tradeID, OrderID bidOrderID, OrderID askOrderID, Price bidPrice,
        Price askPrice, Quantity quantity, MatchType matchType)
      : tradeID_(tradeID), bidOrderID_(bidOrderID), askOrderID_(askOrderID),
        bidPrice_(bidPrice), askPrice_(askPrice), quantity_(quantity),
        matchType_(matchType), timestamp_(GetCurrentTime()) {}

  const TradeID GetTradeID() const { return tradeID_; }
  const OrderID GetBidOrderID() const { return bidOrderID_; }
  const OrderID GetAskOrderID() const { return askOrderID_; }
  Time GetTimestamp() const { return timestamp_; }
  Price GetAskPrice() const { return askPrice_; }
  Price GetBidPrice() const { return bidPrice_; }
  Quantity GetQuantity() const { return quantity_; }
  MatchType GetMatchType() const { return matchType_; }

  void info() const {
    std::cout << "TradeID: " << tradeID_ << ", bidOrderID: " << bidOrderID_
              << ", askOrderID: " << askOrderID_
              << ", Timestamp: " << timestamp_ << ", bidPrice: " << bidPrice_
              << ",askPrice:  " << askPrice_ << ", Quantity: " << quantity_
              << ", Match Type: " << to_string(matchType_) << '\n';
  }

private:
  const TradeID tradeID_;
  const OrderID bidOrderID_, askOrderID_;
  Time timestamp_;
  Price bidPrice_, askPrice_;
  Quantity quantity_;
  MatchType matchType_;
};
} // namespace engine
} // namespace ob