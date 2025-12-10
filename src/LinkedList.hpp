#pragma once

#include <cstddef>

namespace ob {
namespace data {
template <typename T> class LinkedList {
public:
  struct node {
    T data;
    node *next = nullptr, *prev = nullptr;

    node() : data() {}
    explicit node(T _data) : data(_data) {}
    node &operator=(const node &other) {
      data = other.data;
      next = other.next;
      prev = other.prev;
      return *this;
    }

    void clear() { next = prev = nullptr; }
  };

  LinkedList() {}
  ~LinkedList() { clear(); }

  node *insert_front(T data);
  node *insert_back(T data);
  void pop_front();
  void pop_back();
  void remove(node *data);
  void clear();
  T front() const;
  bool empty() const { return size_ == 0; };

private:
  node *root_ = nullptr, *last_ = nullptr;
  size_t size_ = 0;
};
} // namespace data
} // namespace ob