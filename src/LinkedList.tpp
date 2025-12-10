#include <stdexcept>
#include "LinkedList.hpp"

namespace ob {
namespace data {

template <typename T> LinkedList<T>::node *LinkedList<T>::insert_front(T data) {
  if (!root_) {
    root_ = new node(data);
    root_ = last_;
    size_++;
    return root_;
  }

  node *v = new node(data);
  root_->prev = v;
  v->next = root_;
  root_ = v;
  size_++;
  return v;
}

template <typename T> LinkedList<T>::node *LinkedList<T>::insert_back(T data) {
  if (!last_) {
    last_ = new node(data);
    root_ = last_;
    size_++;
    return last_;
  }

  node *v = new node(data);
  last_->next = v;
  v->prev = last_;
  last_ = v;
  size_++;
  return v;
}

template <typename T> void LinkedList<T>::pop_front() {
  if (root_ == nullptr)
    throw std::logic_error("Trying to pop from an empty list.");

  if (last_ == root_) {
    last_->clear(), root_->clear();
    delete root_;
    last_ = root_ = nullptr;
    size_--;
    return;
  }

  node *tmp = root_;
  root_ = root_->next;
  root_->prev = nullptr;

  tmp->clear();
  delete tmp;
  tmp = nullptr;
  size_--;
}

template <typename T> void LinkedList<T>::pop_back() {
  if (last_ == nullptr)
    throw std::logic_error("Trying to pop from an empty list.");

  if (last_ == root_) {
    last_->clear(), root_->clear();
    delete last_;
    last_ = root_ = nullptr;
    size_--;
    return;
  }

  node *tmp = last_;
  last_ = last_->prev;
  last_->next = nullptr;

  tmp->clear();
  delete tmp;
  tmp = nullptr;
  size_--;
}

template <typename T> void LinkedList<T>::remove(node *data) {
  if (!data)
    throw std::logic_error("Trying to remove a nullptr from the linked list.");
  if (size_ == 1) {
    if (data != root_ || !root_)
      throw std::logic_error("Bug");

    root_->clear(), last_->clear(), data->clear();
    delete root_;
    root_ = last_ = data = nullptr;
    size_ = 0;
    return;
  }

  node *prv = data->prev, *nxt = data->next;
  if (prv)
    prv->next = nxt;
  if (nxt)
    nxt->prev = prv;

  if (data == last_)
    last_ = prv;
  if (data == root_)
    root_ = nxt;

  data->clear();
  delete data;
  data = nullptr;
  size_--;
}

template <typename T> T LinkedList<T>::front() const {
  if (!root_)
    throw std::logic_error("Trying to access an empty linked list!");
  return root_->data;
}

template <typename T> void LinkedList<T>::clear() {
  node *curr = root_;
  while (curr) {
    node *nxt = curr->next;
    curr->clear();
    delete curr;
    curr = nxt;
  }
  root_ = last_ = nullptr;
  size_ = 0;
}
} // namespace data
} // namespace ob