#include "Orderbook.hpp"
#include <cassert>

namespace ob::engine {
[[nodiscard]] OrderID Orderbook::AddOrder(ClientID clientID, Price price,
                                          Quantity quantity, Side side,
                                          OrderType order_type, TimeInForce tif,
                                          Flags flag) {
  static OrderID orderID_counter = 1;
  AddOrderInternal(Order{orderID_counter, clientID, price, quantity, side,
                         order_type, tif, flag});
  orderID_counter++;
  return orderID_counter;
}

void Orderbook::ModifyOrder(OrderID orderID, Price new_price,
                            Quantity new_quantity) {
  auto it = m_Orders.find(orderID);
  if (it == m_Orders.end()) {
    std::cout << "[INFO]" << "Order: " << orderID
              << "not found to be modified\n";
    return;
  }

  auto &order = *(it->second.iter);
  if (new_price == order.GetPrice() &&
      new_quantity < order.GetFilledQuantity()) {
    order.ModifyOrder(new_price, new_quantity);
    return;
  }

  ClientID cid = order.GetClientID();
  Side side = order.GetSide();
  OrderType type = order.GetOrderType();
  TimeInForce tif = order.GetTimeInForce();
  Flags flags = order.GetFlags();

  CancelOrder(orderID);
  auto tmp = AddOrder(cid, new_price, new_quantity, side, type, tif, flags);
}

void Orderbook::CancelOrder(OrderID id) {
  auto it = m_Orders.find(id);
  if (it == m_Orders.end()) {
    std::cout << "[INFO]" << "Order: " << id << " not found\n";
    return;
  }

  auto &iter = it->second.iter;
  auto &orderList = *(it->second.list_ptr);

  Price price = iter->GetPrice();
  Side side = iter->GetSide();

  orderList.erase(iter);
  m_Orders.erase(it);

  if (orderList.empty()) {
    if (side == Side::Buy)
      m_Bids.erase(price);
    else
      m_Asks.erase(price);
  }
}

// TODO:
void Orderbook::CancelAllOrders() {}

void Orderbook::RemoveFillAndKill() {
  if (!m_Bids.empty()) {
    auto &[_, bids] = *m_Bids.begin();
    auto order = bids.front();
    if (order.GetTimeInForce() == TimeInForce::FillAndKill)
      CancelOrder(order.GetOrderID());
  }

  if (!m_Asks.empty()) {
    auto &[_, asks] = *m_Asks.begin();
    auto &order = asks.front();
    if (order.GetTimeInForce() == TimeInForce::FillAndKill)
      CancelOrder(order.GetOrderID());
  }
}

[[nodiscard]] bool Orderbook::HasOrders() const noexcept {
  return !m_Bids.empty() && !m_Asks.empty();
}

[[nodiscard]] std::optional<Order> Orderbook::GetOrder(OrderID orderID) const {
  auto it = m_Orders.find(orderID);
  if (it == m_Orders.end())
    return std::nullopt;
  return *(it->second.iter);
}

Order *Orderbook::GetBestBid() {
  if (m_Bids.empty())
    return nullptr;
  return &(m_Bids.begin()->second.front());
}

Order *Orderbook::GetBestAsk() {
  if (m_Asks.empty())
    return nullptr;
  return &(m_Asks.begin()->second.front());
}
// TODO:
Quantity Orderbook::GetVolumeAtPrice(Price price) const { return Quantity(0); }

// TODO:
std::vector<Order> Orderbook::GetOrderByPrice(Price price) const { return {}; }

// TODO:
Price Orderbook::GetSpread() const { return Price(0); }

// TODO:
void Orderbook::GetDepth() {}

OrderbookSnapshot Orderbook::GetSnapshot(uint32_t depth) const {
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

void Orderbook::PrintOrderbook() {
  std::cout << "------------------\n";
  for (auto &[orderID, OrderPointer] : m_Orders)
    OrderPointer.iter->info();
  std::cout << "------------------\n";
}

void Orderbook::Shutdown() noexcept {
  m_Orders.clear();
  m_Bids.clear();
  m_Asks.clear();
}

void Orderbook::MatchIncomingOrders(Order &order) {
  auto trades = m_MatchingEngine->Match(order, *this);
  for (const auto &trade : trades)
    m_Trades.emplace_back(std::move(trade));
}

void Orderbook::AddOrderInternal(Order &&order) {
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
}
} // namespace ob::engine
