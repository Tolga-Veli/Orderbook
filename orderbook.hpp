#include <map>
#include <queue>
#include <chrono>
#include <random>
#include <exception>
#include <format>
#include <memory>

#include "globals.hpp"

namespace Tools {
class Order {
public:
  Order() = delete;
  Order(OrderID _orderID, Price price, Quantity quantity, Side side)
      : _orderID(_orderID), _time(Tools::GetCurrentTime()), _price(price),
        _initial_quantity(quantity), _remaining_quantity(quantity),
        _side(side) {}
  ~Order() {}

  const OrderID &GetOrderID() const { return _orderID; }
  Price GetPrice() const { return _price; }
  Quantity GetInitialQuantity() const { return _initial_quantity; }
  Quantity GetRemainingQuantity() const { return _remaining_quantity; }
  Quantity GetFilledQuantity() const {
    return _remaining_quantity - _initial_quantity;
  }
  Time GetTime() const { return _time; }
  Side GetSide() const { return _side; }
  bool isFilled() const { return _remaining_quantity == 0; }

  void Fill(Quantity quantity) {
    if (quantity > _remaining_quantity)
      throw std::logic_error(std::format(
          "Order ({}) cannot be filled for more than its capcity.", _orderID));

    _remaining_quantity -= quantity;
  }

private:
  OrderID _orderID;
  Price _price;
  Quantity _initial_quantity, _remaining_quantity;
  Time _time;
  Side _side;
};

class Orderbook {
public:
  void AddOrder(OrderID orderID, Price price, Quantity quantity, Side side);
  void KillOrder(const OrderID &id);

  Orderbook() {}
  ~Orderbook() {}

private:
  std::map<Price, std::queue<OrderID>, std::greater<Price>> _bids;
  std::map<Price, std::queue<OrderID>, std::less<Price>> _asks;
  std::map<OrderID, Order> _orders;
  Time _closingTime;
};
}; // namespace Tools