#include "Orderbook.hpp"
#include <format>
#include <stdexcept>

namespace ob {
namespace engine {
void Orderbook::AddOrder(ClientID clientID, Price price, Quantity quantity,
                         Side side, OrderType order_type, TimeInForce tif,
                         Flags flag) {
  static OrderID orderID_counter = 1;
  AddOrderInternal(Order{orderID_counter++, clientID, price, quantity, side,
                         order_type, tif, flag});
}

std::optional<Order> Orderbook::GetOrder(OrderID orderID) {
  if (orders_.count(orderID) == 0)
    return std::nullopt;
  return orders_[orderID]->data;
}

void Orderbook::ModifyOrder(const OrderID &orderID, Price new_price,
                            Quantity new_quantity) {
  auto it = orders_.find(orderID);
  if (it == orders_.end())
    throw std::logic_error(std::format(
        "Order with ID: {} doesn't exist to be modified.", orderID));

  auto *iter = it->second;
  if (!iter)
    throw std::logic_error(
        std::format("Tried accessing a nullptr iterator to an order."));

  if (new_price == iter->data.GetPrice()) {
    iter->data.ModifyOrder(new_price, new_quantity);
    return;
  }

  Order order = iter->data;
  KillOrder(orderID);
  order.ModifyOrder(new_price, new_quantity);
  AddOrderInternal(order);
}

void Orderbook::KillOrder(const OrderID &id) {
  auto it = orders_.find(id);
  if (it == orders_.end())
    throw std::logic_error(
        std::format("Order with ID: {} doesn't exist to be killed.", id));

  auto *iter = it->second;
  if (!iter)
    throw std::logic_error(
        std::format("Tried accessing a nullptr iterator to an order."));

  Order order = iter->data;
  auto price = order.GetPrice();
  auto side = order.GetSide();

  if (side == Side::Buy) {
    bids_[price].remove(iter);
    if (bids_[price].empty())
      bids_.erase(price);
  } else {
    asks_[price].remove(iter);
    if (asks_[price].empty())
      asks_.erase(price);
  }
  orders_.erase(id);
}

// TODO:
void Orderbook::CancelAllOrders() {}

void Orderbook::MatchOrders() {}

Order Orderbook::GetBestBid() {
  if (bids_.empty())
    throw std::logic_error("Trying to access an empty bids book.");
  return bids_.begin()->second.front();
}

Order Orderbook::GetBestAsk() {
  if (asks_.empty())
    throw std::logic_error("Trying to access an empty asks book.");
  return asks_.begin()->second.front();
}

// TODO:
std::vector<Order> Orderbook::GetOrderByPrice(Price price) { return {}; }

// TODO:
std::vector<Trade> Orderbook::GetTradeHistory() { return {}; }

// TODO:
Quantity Orderbook::GetVolumeAtPrice(Price price) { return Quantity(0); }

// TODO:
Price Orderbook::GetSpread() { return Price(0); }

// TODO:
void Orderbook::GetDepth() {}

void Orderbook::PrintOrderbook() {
  std::cout << "------------------\n";
  for (auto &[orderID, order_iter] : orders_) {
    const Order &data = order_iter->data;
    data.print();
  }
  std::cout << "------------------\n";
}

void Orderbook::Shutdown() {
  orders_.clear();
  bids_.clear();
  asks_.clear();
}

void Orderbook::AddOrderInternal(Order order) {
  if (orders_.count(order.GetOrderID()))
    throw std::logic_error(
        std::format("Duplicate order IDs {}.", order.GetOrderID()));

  data::LinkedList<Order>::node *node;
  if (order.GetSide() == Side::Sell)
    node = asks_[order.GetPrice()].insert_back(order);
  else
    node = bids_[order.GetPrice()].insert_back(order);
  orders_[order.GetOrderID()] = node;
}
} // namespace engine
} // namespace ob