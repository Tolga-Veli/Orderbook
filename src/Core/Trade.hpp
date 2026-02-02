#pragma once

#include "globals.hpp"

#include <iostream>

namespace ob {
class Trade {
public:
  Trade() = delete;
  Trade(TradeID tradeID, OrderID bidOrderID, OrderID askOrderID, Price bidPrice,
        Price askPrice, Quantity quantity, MatchType matchType)
      : m_TradeID(tradeID), m_BidOrderID(bidOrderID), m_AskOrderID(askOrderID),
        m_BidPrice(bidPrice), m_AskPrice(askPrice), m_Quantity(quantity),
        m_MatchType(matchType), m_Timestamp(core::GetCurrentTime()) {}

  const TradeID GetTradeID() const noexcept { return m_TradeID; }
  const OrderID GetBidOrderID() const noexcept { return m_BidOrderID; }
  const OrderID GetAskOrderID() const noexcept { return m_AskOrderID; }
  Time GetTimestamp() const noexcept { return m_Timestamp; }
  Price GetAskPrice() const noexcept { return m_AskPrice; }
  Price GetBidPrice() const noexcept { return m_BidPrice; }
  Quantity GetQuantity() const noexcept { return m_Quantity; }
  MatchType GetMatchType() const noexcept { return m_MatchType; }

  void info() const {
    std::cout << "TradeID: " << m_TradeID << ", bidOrderID: " << m_BidOrderID
              << ", askOrderID: " << m_AskOrderID
              << ", Timestamp: " << m_Timestamp << ", bidPrice: " << m_BidPrice
              << ",askPrice:  " << m_AskPrice << ", Quantity: " << m_Quantity
              << ", Match Type: " << core::to_string(m_MatchType) << '\n';
  }

private:
  TradeID m_TradeID;
  OrderID m_BidOrderID, m_AskOrderID;
  Time m_Timestamp;
  Price m_BidPrice, m_AskPrice;
  Quantity m_Quantity;
  MatchType m_MatchType;
};
} // namespace ob
