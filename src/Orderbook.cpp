#include <cassert>
#include "Orderbook.hpp"

namespace ob {
namespace engine {
void Orderbook::AddOrder(ClientID clientID, Price price, Quantity quantity,
                         Side side, OrderType order_type, TimeInForce tif,
                         Flags flag) {
  static OrderID orderID_counter = 1;
  AddOrderInternal(Order{orderID_counter, clientID, price, quantity, side,
                         order_type, tif, flag});
  orderID_counter++;
}

void Orderbook::ModifyOrder(OrderID orderID, Price new_price,
                            Quantity new_quantity) {
  auto it = orders_.find(orderID);
  if (it == orders_.end()) {
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
  AddOrder(cid, new_price, new_quantity, side, type, tif, flags);
}

void Orderbook::CancelOrder(OrderID id) {
  auto it = orders_.find(id);
  if (it == orders_.end()) {
    std::cout << "[INFO]" << "Order: " << id << " not found\n";
    return;
  }

  auto &iter = it->second.iter;
  auto &orderList = *(it->second.list_ptr);

  Price price = iter->GetPrice();
  Side side = iter->GetSide();

  orderList.erase(iter);
  orders_.erase(it);

  if (orderList.empty()) {
    if (side == Side::Buy)
      bids_.erase(price);
    else
      asks_.erase(price);
  }
}

// TODO:
void Orderbook::CancelAllOrders() {}

void Orderbook::RemoveFillAndKill() {
  if (!bids_.empty()) {
    auto &[_, bids] = *bids_.begin();
    auto order = bids.front();
    if (order.GetTimeInForce() == TimeInForce::FillAndKill)
      CancelOrder(order.GetOrderID());
  }

  if (!asks_.empty()) {
    auto &[_, asks] = *asks_.begin();
    auto &order = asks.front();
    if (order.GetTimeInForce() == TimeInForce::FillAndKill)
      CancelOrder(order.GetOrderID());
  }
}

[[nodiscard]] bool Orderbook::HasOrders() const {
  return !bids_.empty() && !asks_.empty();
}

[[nodiscard]] std::optional<std::reference_wrapper<Order>>
Orderbook::GetOrder(OrderID orderID) const {
  auto it = orders_.find(orderID);
  if (it == orders_.end())
    return std::nullopt;
  return *(it->second.iter);
}

Order *Orderbook::GetBestBid() const {
  if (bids_.empty())
    return nullptr;
  return &(bids_.begin()->second.front());
}

Order *Orderbook::GetBestAsk() const {
  if (asks_.empty())
    return nullptr;
  return &(asks_.begin()->second.front());
}
// TODO:
Quantity Orderbook::GetVolumeAtPrice(Price price) const { return Quantity(0); }

// TODO:
std::vector<Order> Orderbook::GetOrderByPrice(Price price) const { return {}; }

// TODO:
Price Orderbook::GetSpread() const { return Price(0); }

// TODO:
void Orderbook::GetDepth() {}

void Orderbook::PrintOrderbook() {
  std::cout << "------------------\n";
  for (auto &[orderID, OrderPointer] : orders_)
    OrderPointer.iter->info();
  std::cout << "------------------\n";
}

void Orderbook::Shutdown() {
  orders_.clear();
  bids_.clear();
  asks_.clear();
}

void Orderbook::MatchIncomingOrders(Order &order) {
  auto trades = matchingEngine_->Match(order, *this);
  for (const auto &trade : trades)
    trades_.emplace_back(std::move(trade));
}

void Orderbook::AddOrderInternal(Order &&order) {
  if (orders_.count(order.GetOrderID()))
    return;

  data::List<Order> *list_ptr = nullptr;
  data::List<Order>::iterator it;

  if (order.GetSide() == Side::Buy) {
    auto &list = bids_[order.GetPrice()];
    it = list.push_back(std::move(order));
    list_ptr = &list;
  } else {
    auto &list = asks_[order.GetPrice()];
    it = list.push_back(std::move(order));
    list_ptr = &list;
  }
  orders_[order.GetOrderID()] = {it, list_ptr};
  MatchIncomingOrders(order);
}

} // namespace engine
} // namespace ob