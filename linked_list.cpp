#include "linked_list.hpp"

namespace Tools {
template <typename T> LinkedList<T>::node *LinkedList<T>::insert_back(T data) {
  node *v = new node(data);
  last->next = v;
  v->prev = last;
  last = v;
  return v;
}

template <typename T> void LinkedList<T>::pop_back() {
  if (last == root)
    throw std::logic_error("Trying to pop from an empty list.");

  node *tmp = last;
  last = last->prev;
  last->next = nullptr;

  tmp->prev = tmp->next = nullptr;
  delete tmp;
}

template <typename T> void LinkedList<T>::remove(node *data) {
  if (!data)
    throw std::logic_error("Trying to remove a nullptr from the linked list.");

  node *prv = data->prev, *nxt = data->next;
  if (prv)
    prv->next = nxt;
  if (nxt)
    nxt->prev = prv;
  if (data == last)
    last = prv;

  data->prev = data->next = nullptr;
  delete data;
}

template <typename T> void LinkedList<T>::clear() {
  node *curr = root;
  while (curr) {
    node *nxt = curr->next;
    curr->prev = curr->next = nullptr;
    delete curr;
    curr = nxt;
  }
  last->prev = last->next = nullptr;
  delete last;
}
}; // namespace Tools