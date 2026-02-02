#include "OrderBook.hpp"
#include <cassert>

namespace ob::engine {

bool OrderBook::HasOrders() const noexcept {
  return !m_Bids.empty() && !m_Asks.empty();
}

std::optional<Order> OrderBook::GetOrder(OrderID orderID) const {
  auto it = m_Orders.find(orderID);
  if (it == m_Orders.end())
    return std::nullopt;
  return *(it->second.iter);
}

Order *OrderBook::GetBestBid() {
  if (m_Bids.empty())
    return nullptr;
  return &(m_Bids.begin()->second.front());
}

Order *OrderBook::GetBestAsk() {
  if (m_Asks.empty())
    return nullptr;
  return &(m_Asks.begin()->second.front());
}
// TODO:
Quantity OrderBook::GetBidVolumeAtPrice(Price price) const {
  Quantity total = 0;
  auto it = m_Bids.find(price);
  if (it != m_Bids.end())
    for (const auto &order : it->second)
      total += order.GetRemainingQuantity();

  return total;
}

Quantity OrderBook::GetAskVolumeAtPrice(Price price) const {
  Quantity total = 0;
  auto it = m_Asks.find(price);
  if (it != m_Asks.end())
    for (const auto &order : it->second)
      total += order.GetRemainingQuantity();

  return total;
}

OrderBookSnapshot OrderBook::GetSnapshot(uint32_t depth) const {
  uint32_t curr_depth = 0;
  std::vector<std::pair<Price, Quantity>> bids, asks;
  for (const auto &[price, list] : m_Bids) {
    if (++curr_depth > depth)
      break;

    Quantity total_quantity = 0;
    for (const auto &order : list)
      total_quantity += order.GetRemainingQuantity();

    bids.push_back({price, total_quantity});
  }

  curr_depth = 0;
  for (const auto &[price, list] : m_Asks) {
    if (++curr_depth > depth)
      break;

    Quantity total_quantity = 0;
    for (const auto &order : list)
      total_quantity += order.GetRemainingQuantity();

    asks.push_back({price, total_quantity});
  }

  return {bids, asks, m_Trades};
}

void OrderBook::PrintOrderBook() const {
  std::cout << "------------------\n";
  for (const auto &[orderID, OrderPointer] : m_Orders)
    OrderPointer.iter->info();
  std::cout << "------------------\n";
}

void OrderBook::Handle(const cmd::AddOrder &add) {
  const auto &[clientID, clientOrderID, price, quantity, side, order_type, tif,
               flags] = add;

  static OrderID orderID_counter = 1;
  const Order order{orderID_counter, clientID, price, quantity, side,
                    order_type,      tif,      flags};

  if (m_Orders.count(order.GetOrderID()))
    return;

  OrderPointer op{};
  if (order.GetSide() == Side::Buy) {
    auto [it, inserted] = m_Bids.try_emplace(
        order.GetPrice(), std::pmr::list<Order>(&m_Resource));
    auto &list = it->second;

    list.push_back(std::move(order));
    op.iter = std::prev(list.end());
    op.list_ptr = &list;
  } else {
    auto [it, inserted] = m_Asks.try_emplace(
        order.GetPrice(), std::pmr::list<Order>(&m_Resource));
    auto &list = it->second;

    list.push_back(std::move(order));
    op.iter = std::prev(list.end());
    op.list_ptr = &list;
  }
  const OrderID id = op.iter->GetOrderID();
  m_Orders[id] = op;

  MatchIncomingOrders(*op.iter);
  return;
}

void OrderBook::Handle(const cmd::ModifyOrder &modify) {
  const auto &[orderID, new_price, new_quantity] = modify;
  auto it = m_Orders.find(orderID);
  auto &order = *(it->second.iter);
  if (new_price == order.GetPrice() &&
      new_quantity < order.GetFilledQuantity()) {
    order.ModifyOrder(new_price, new_quantity);
  } else
    throw std::logic_error("ModifyOrder:: The new quantity must be >= then the "
                           "order's filled quantity");
}

void OrderBook::Handle(const cmd::CancelOrder &cancel) {
  const auto &orderID = cancel.orderID;
  auto it = m_Orders.find(orderID);
  auto &iter = it->second.iter;
  auto &orderList = *(it->second.list_ptr);

  Price price = iter->GetPrice();
  Side side = iter->GetSide();

  orderList.erase(iter);
  m_Orders.erase(it);

  if (!orderList.empty())
    return;

  if (side == Side::Buy)
    m_Bids.erase(price);
  else
    m_Asks.erase(price);
}

void OrderBook::MatchIncomingOrders(Order &order) {
  auto trades = m_MatchingEngine->Match(order, *this);
  for (const auto &trade : trades)
    m_Trades.emplace_back(std::move(trade));
}

} // namespace ob::engine
