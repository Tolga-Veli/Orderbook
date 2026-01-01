#include "Orderbook.hpp"
#include <random>
#include <vector>
#include <list>
#include <iostream>

int main() {

  std::mt19937 rng{1};
  std::uniform_int_distribution<int> dist(1, 100);
  int t = 1;
  while (t--) {
    ob::engine::Orderbook orderbook;
    using ob::engine::ClientID;
    using ob::engine::Flags;
    using ob::engine::OrderType;
    using ob::engine::Side;
    using ob::engine::TimeInForce;

    for (int i = 0; i < 10; i++)
      orderbook.AddOrder(ClientID{1}, dist(rng), dist(rng), Side::Sell,
                         OrderType::Limit, TimeInForce::GoodTillCancelled,
                         Flags::None);

    std::vector<bool> vis(11);
    std::vector<int> removed;
    std::uniform_int_distribution<int> d(1, 10);
    for (int i = 0; i < 5; i++) {
      int curr = d(rng);
      while (vis[curr])
        curr = d(rng);

      std::optional<std::reference_wrapper<ob::engine::Order>> res =
          orderbook.GetOrder(curr);
      if (res.has_value()) {
        orderbook.CancelOrder(curr);
        vis[curr] = true;
        removed.push_back(curr);
      }
    }
    std::cout << "\n---------------\n";
    std::cout << "Removed values:\n";
    std::sort(removed.begin(), removed.end());
    for (int val : removed)
      std::cout << val << " ";

    orderbook.PrintOrderbook();

    for (int i = 0; i < 10; i++)
      orderbook.AddOrder(ClientID{2}, dist(rng), dist(rng), Side::Buy,
                         OrderType::Limit, TimeInForce::GoodTillCancelled,
                         Flags::None);

    orderbook.PrintOrderbook();
    auto Trades = orderbook.GetTradeHistory();
    for (auto &Trade : Trades)
      Trade.info();
  }
}