#include <cassert>
#include "List.hpp"

namespace ob {
namespace data {
template <typename value_type>
template <typename T>
List<value_type>::iterator List<value_type>::push_front(T &&data) {
  if (!head_) {
    head_ = tail_ = new Node(std::forward<value_type>(data));
    size_++;
    return iterator(head_);
  }

  Node *v = new Node(std::forward<value_type>(data));
  head_->prev = v;
  v->next = head_;
  head_ = v;
  size_++;
  return iterator(v);
}

template <typename value_type>
template <typename T>
List<value_type>::iterator List<value_type>::push_back(T &&data) {
  if (!tail_) {
    head_ = tail_ = new Node(std::forward<value_type>(data));
    size_++;
    return iterator(tail_);
  }

  Node *v = new Node(std::forward<value_type>(data));
  v->prev = tail_;
  tail_->next = v;
  tail_ = v;
  size_++;
  return iterator(v);
}

template <typename value_type> void List<value_type>::pop_front() {
  assert(head_ != nullptr && "Trying to erase an element from an empty list");
  if (tail_ == head_) {
    tail_->clear(), head_->clear();
    delete head_;
    tail_ = head_ = nullptr;
    size_--;
    return;
  }

  Node *tmp = head_;
  head_ = head_->next;
  head_->prev = nullptr;

  tmp->clear();
  delete tmp;
  size_--;
}

template <typename value_type> void List<value_type>::pop_back() {
  assert(tail_ != nullptr && "Trying to erase an element from an empty list.");
  if (tail_ == head_) {
    tail_->clear(), head_->clear();
    delete tail_;
    tail_ = head_ = nullptr;
    size_--;
    return;
  }

  Node *tmp = tail_;
  tail_ = tail_->prev;
  tail_->next = nullptr;
  tmp->clear();
  delete tmp;
  size_--;
}

template <typename value_type>
void List<value_type>::erase(const iterator &it) {
  assert(size_ > 0);
  Node *data = it.base();
  if (size_ == 1) {
    data->clear();
    delete data;
    head_ = tail_ = nullptr;
    size_ = 0;
    return;
  }

  Node *prv = data->prev, *nxt = data->next;
  if (prv)
    prv->next = nxt;
  if (nxt)
    nxt->prev = prv;

  if (data == tail_)
    tail_ = prv;
  if (data == head_)
    head_ = nxt;

  data->clear();
  delete data;
  data = nullptr;
  size_--;
}

template <typename value_type> void List<value_type>::clear() {
  Node *curr = head_;
  while (curr) {
    Node *nxt = curr->next;
    curr->clear();
    delete curr;
    curr = nxt;
  }
  head_ = tail_ = nullptr;
  size_ = 0;
}

template <typename value_type> value_type &List<value_type>::front() const {
  assert(head_ != nullptr && "Trying to access the front of an empty list");
  return head_->data;
}

template <typename value_type> value_type &List<value_type>::back() const {
  assert(tail_ != nullptr && "Trying to access the back of an empty list");
  return tail_->data;
}

} // namespace data
} // namespace ob