#include <cstdint>
#include <chrono>

namespace Tools {
using OrderID = uint64_t;
using Price = int64_t;
using Quantity = size_t;
using Time = std::chrono::nanoseconds;
using OrderPointer = std::shared_ptr<Order>;

enum class Side { Buy, Sell };
enum class OrderType { Limit, Market, Stop, StopLimit };
enum class TimeInForce {
  GoodForDay,
  GoodTillCancelled,
  ImmediateOrCancel,
  FillOrKill
};
enum class Flags { None = 0, PostOnly, Hidden };

static Time GetCurrentTime() {
  return std::chrono::duration_cast<Time>(
      std::chrono::steady_clock::now().time_since_epoch());
}

}; // namespace Tools