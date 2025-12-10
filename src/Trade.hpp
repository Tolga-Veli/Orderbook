#include "globals.hpp"

namespace ob {
namespace core {
class Trade {
public:
  Trade() = delete;
  Trade(TradeID tradeID, OrderID makerOrderID, OrderID takerOrderID,
        Price price, Quantity quantity, Side takerSide,
        LiquidityFlag liquidityFlag, MatchType matchType)
      : tradeID_(tradeID), makerOrderID_(makerOrderID),
        timestamp_(util::GetCurrentTime()), takerOrderID_(takerOrderID),
        price_(price), quantity_(quantity), takerSide_(takerSide),
        liquidityFlag_(liquidityFlag), matchType_(matchType) {}

  const TradeID GetTradeID() const { return tradeID_; }
  const OrderID GetMakerOrderID() const { return makerOrderID_; }
  const OrderID GetTakerOrderID() const { return takerOrderID_; }
  Time GetTimestamp() const { return timestamp_; }
  Price GetPrice() const { return price_; }
  Quantity GetQuantity() const { return quantity_; }
  Side GetSide() const { return takerSide_; }
  LiquidityFlag GetLiquidityFlag() const { return liquidityFlag_; }
  MatchType GetMatchType() const { return matchType_; }

private:
  const TradeID tradeID_;
  const OrderID makerOrderID_, takerOrderID_;
  Time timestamp_;
  Price price_;
  Quantity quantity_;
  Side takerSide_;
  LiquidityFlag liquidityFlag_;
  MatchType matchType_;
};
} // namespace core
} // namespace ob