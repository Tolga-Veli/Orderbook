#include "Orderbook.hpp"
#include <random>

int main() {

  std::mt19937 rng{1};
  std::uniform_int_distribution<int> dist(1, 100);
  int t = 1;
  while (t--) {
    ob::engine::Orderbook orderbook;
    using ob::core::ClientID;
    using ob::core::Flags;
    using ob::core::OrderType;
    using ob::core::Side;
    using ob::core::TimeInForce;

    for (int i = 0; i < 10; i++)
      orderbook.AddOrder(dist(rng), ClientID{1}, dist(rng), Side::Buy,
                         OrderType::Limit, TimeInForce::GoodTillCancelled,
                         Flags::None);
    for (int i = 0; i < 10; i++)
      orderbook.AddOrder(dist(rng), ClientID{2}, dist(rng), Side::Sell,
                         OrderType::Limit, TimeInForce::GoodTillCancelled,
                         Flags::None);

    orderbook.PrintOrderbook();
    std::vector<bool> vis(21);
    std::vector<int> removed;
    std::uniform_int_distribution<int> d(1, 20);
    for (int i = 0; i < 5; i++) {
      int curr = d(rng);
      while (vis[curr])
        curr = d(rng);

      auto res = orderbook.GetOrder(curr);
      if (res.has_value()) {
        orderbook.KillOrder(curr);
        vis[curr] = true;
        removed.push_back(curr);
      }
    }

    std::cout << "\n---------------\n";
    std::cout << "Removed values:\n";
    std::sort(removed.begin(), removed.end());
    for (int val : removed)
      std::cout << val << " ";
    std::cout << "\n---------------\n";
    orderbook.PrintOrderbook();
  }
}