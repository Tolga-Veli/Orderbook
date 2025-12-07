#include "orderbook.hpp"

namespace Tools {

void Orderbook::AddOrder(OrderID orderID, Price price, Quantity quantity,
                         Side side) {
  if (_orders.count(orderID))
    throw std::logic_error(std::format("Duplicate order IDs {}.", orderID));

  Order order{orderID, price, quantity, side};
  _orders[orderID] = order;

  if (side == Side::Sell)
    _asks[price].push(orderID);
  else
    _bids[price].push(orderID);
}

void Orderbook::KillOrder(const OrderID &id) {
  auto it = _orders.find(id);
  if (it == _orders.end())
    throw std::logic_error(
        std::format("Order with ID: {} doesn't exist to be killed.", id));

  auto &order = it->second;
  auto price = order.GetPrice();
  auto remaining_quantity = order.GetRemainingQuantity();

  auto &old_queue =
      (order.GetSide() == Side::Buy ? _bids[price] : _asks[price]);
  std::queue<OrderID> new_queue;
  while (!old_queue.empty()) {
    auto order_id = old_queue.front();
    old_queue.pop();

    if (order_id != id)
      new_queue.push(order_id);
  }
  std::swap(old_queue, new_queue);
  _orders.erase(it);
}
}; // namespace Tools