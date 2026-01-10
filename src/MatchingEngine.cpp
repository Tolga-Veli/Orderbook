#include "MatchingEngine.hpp"
#include "Orderbook.hpp"

namespace ob {
namespace engine {
uint32_t IMatchingEngine::counter = 0;

std::vector<Trade> FIFO_Matching::Match(Order &order, Orderbook &book) {
  std::vector<Trade> trades;
  while (true) {
    if (!book.HasOrders())
      break;

    auto bid = book.GetBestBid(), ask = book.GetBestAsk();
    if (!bid || !ask)
      break;

    auto bidPrice = bid->GetPrice(), askPrice = ask->GetPrice();
    if (bidPrice < askPrice)
      break;

    Quantity quantity =
        std::min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity());

    bid->Fill(quantity);
    ask->Fill(quantity);

    if (bid->isFilled())
      book.CancelOrder(bid->GetOrderID());
    if (ask->isFilled())
      book.CancelOrder(ask->GetOrderID());

    trades.emplace_back(std::move(
        Trade{counter++, bid->GetOrderID(), ask->GetOrderID(), bid->GetPrice(),
              ask->GetPrice(), quantity, MatchType::Standard}));
  }

  book.RemoveFillAndKill();
  return trades;
}
} // namespace engine
} // namespace ob