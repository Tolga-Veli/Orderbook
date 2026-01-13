#include <cassert>
#include "List.hpp"

namespace ob::data {
template <typename value_type>
template <typename T>
List<value_type>::iterator List<value_type>::push_front(T &&data) {
  if (!m_Head) {
    m_Head = m_Tail = new Node(std::forward<T>(data));
    m_Size++;
    return iterator(m_Head);
  }

  Node *v = new Node(std::forward<T>(data));
  m_Head->prev = v;
  v->next = m_Head;
  m_Head = v;
  m_Size++;
  return iterator(v);
}

template <typename value_type>
template <typename T>
List<value_type>::iterator List<value_type>::push_back(T &&data) {
  if (!m_Tail) {
    m_Head = m_Tail = new Node(std::forward<T>(data));
    m_Size++;
    return iterator(m_Tail);
  }

  Node *v = new Node(std::forward<T>(data));
  v->prev = m_Tail;
  m_Tail->next = v;
  m_Tail = v;
  m_Size++;
  return iterator(v);
}

template <typename value_type> void List<value_type>::pop_front() {
  assert(m_Head != nullptr && "Trying to erase an element from an empty list");
  if (m_Tail == m_Head) {
    m_Tail->clear(), m_Head->clear();
    delete m_Head;
    m_Tail = m_Head = nullptr;
    m_Size--;
    return;
  }

  Node *tmp = m_Head;
  m_Head = m_Head->next;
  m_Head->prev = nullptr;

  tmp->clear();
  delete tmp;
  m_Size--;
}

template <typename value_type> void List<value_type>::pop_back() {
  assert(m_Tail != nullptr && "Trying to erase an element from an empty list.");
  if (m_Tail == m_Head) {
    m_Tail->clear(), m_Head->clear();
    delete m_Tail;
    m_Tail = m_Head = nullptr;
    m_Size--;
    return;
  }

  Node *tmp = m_Tail;
  m_Tail = m_Tail->prev;
  m_Tail->next = nullptr;
  tmp->clear();
  delete tmp;
  m_Size--;
}

template <typename value_type>
void List<value_type>::erase(const iterator &it) {
  assert(m_Size > 0 && "Trying to erase an element from an empty list.");
  Node *data = it.base();
  if (m_Size == 1) {
    data->clear();
    delete data;
    m_Head = m_Tail = nullptr;
    m_Size = 0;
    return;
  }

  Node *prv = data->prev, *nxt = data->next;
  if (prv)
    prv->next = nxt;
  if (nxt)
    nxt->prev = prv;

  if (data == m_Tail)
    m_Tail = prv;
  if (data == m_Head)
    m_Head = nxt;

  data->clear();
  delete data;
  m_Size--;
}

template <typename value_type> void List<value_type>::clear() {
  Node *curr = m_Head;
  while (curr) {
    Node *nxt = curr->next;
    curr->clear();
    delete curr;
    curr = nxt;
  }
  m_Head = m_Tail = nullptr;
  m_Size = 0;
}

template <typename value_type> value_type &List<value_type>::front() const {
  assert(m_Head != nullptr && "Trying to access the front of an empty list");
  return m_Head->data;
}

template <typename value_type> value_type &List<value_type>::back() const {
  assert(m_Tail != nullptr && "Trying to access the back of an empty list");
  return m_Tail->data;
}
} // namespace ob::data