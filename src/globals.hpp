#pragma once

#include <cstdint>
#include <chrono>
#include <iostream>
#include <string_view>

namespace ob {
namespace core {
using OrderID = uint64_t;
using ClientID = uint64_t;
using TradeID = uint64_t;
using Price = int64_t; // in cents therefore  $1 = 100
using Quantity = uint64_t;
using Time = std::chrono::nanoseconds;

enum class Side { Buy, Sell };

enum class OrderType { Limit, Market, Stop, StopLimit };

enum class TimeInForce {
  GoodForDay,
  GoodTillCancelled,
  ImmediateOrCancel,
  FillOrKill
};
enum class Flags : uint8_t {
  None = 0,
  Hidden = 1,
  Iceberg = 1 << 1,
  PostOnly = 1 << 2
};

inline Flags operator|(Flags a, Flags b) {
  return static_cast<Flags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline Flags operator&(Flags a, Flags b) {
  return static_cast<Flags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline Flags &operator|=(Flags &a, Flags b) {
  a = a | b;
  return a;
}

enum class LiquidityFlag {
  Maker,
  Taker,
};

enum class MatchType {
  Standard,
  Midpoint,
  HiddenLiquidity,
  Auction,
};

inline constexpr std::string_view to_string(Side side) {
  if (side == Side::Buy)
    return "Buy";
  else
    return "Sell";
}

inline constexpr std::string_view to_string(OrderType order_type) {
  switch (order_type) {
  case OrderType::Limit:
    return "Limit";
  case OrderType::Market:
    return "Market";
  case OrderType::Stop:
    return "Stop";
  case OrderType::StopLimit:
    return "StopLimit";
  default:
    throw std::logic_error("Missed an order type.");
  }
}

inline constexpr std::string_view to_string(TimeInForce tif) {
  switch (tif) {
  case TimeInForce::GoodForDay:
    return "GoodForDay";
  case TimeInForce::GoodTillCancelled:
    return "GoodTillCancelled";
  case TimeInForce::ImmediateOrCancel:
    return "ImmediateOrCancel";
  case TimeInForce::FillOrKill:
    return "FillOrKill";
  default:
    throw std::logic_error("Missed a time in force.");
  }
}

inline std::string to_string(Flags flag) {
  if (flag == Flags::None)
    return "None";

  std::string str = "";
  if ((flag & Flags::Hidden) == Flags::Hidden)
    str += "Hidden|";
  if ((flag & Flags::Iceberg) == Flags::Iceberg)
    str += "Iceberg|";
  if ((flag & Flags::PostOnly) == Flags::PostOnly)
    str += "PostOnly|";

  if (!str.empty())
    str.pop_back();

  return str;
}

inline constexpr std::string_view to_string(LiquidityFlag liquidityFlag) {
  if (liquidityFlag == LiquidityFlag::Maker)
    return "Maker";
  else
    return "Taker";
}

inline constexpr std::string_view to_string(MatchType matchType) {
  switch (matchType) {
  case MatchType::Standard:
    return "Standard";
  case MatchType::Midpoint:
    return "Midpoint";
  case MatchType::HiddenLiquidity:
    return "HiddenLiquidity";
  case MatchType::Auction:
    return "Auction";
  default:
    throw std::logic_error("Match Type doesn't exist.");
  }
}
}; // namespace core

namespace util {
static core::Time GetCurrentTime() {
  return std::chrono::duration_cast<core::Time>(
      std::chrono::steady_clock::now().time_since_epoch());
}

} // namespace util
} // namespace ob