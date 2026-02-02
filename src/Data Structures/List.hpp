#pragma once

#include <memory>
#include <utility>

namespace ob::Memory {
// TODO: Write a memory pool like this one:
// https://8dcc.github.io/programming/pool-allocator.html I have implemented in
// C and it works; now i have to do it in C++

template <class T> class Pool {};
} // namespace ob::Memory

namespace ob::data {
template <typename value_type> class List {
  struct Node {
    value_type data;
    Node *next = nullptr, *prev = nullptr;

    Node() : data() {}
    template <typename T> Node(T &&val) : data(std::forward<T>(val)) {}
    Node(const value_type &val) : data(val) {}

    Node &operator=(const Node &other) {
      data = other.data;
      next = other.next;
      prev = other.prev;
      return *this;
    }
  };

  template <typename data_type, typename pointer_t> struct iterator_base {
  public:
    iterator_base() : pointer_(nullptr) {}
    explicit iterator_base(pointer_t ptr) : pointer_(ptr) {}
    iterator_base(const iterator_base &other) : pointer_(other.pointer_) {}

    iterator_base &operator=(const iterator_base &other) {
      pointer_ = other.pointer_;
      return *this;
    }

    data_type &operator*() const { return pointer_->data; }
    data_type *operator->() const { return &(pointer_->data); }

    iterator_base &operator++() {
      pointer_ = pointer_->next;
      return *this;
    }

    iterator_base operator++(int) {
      iterator_base temp = *this;
      pointer_ = pointer_->next;
      return temp;
    }

    iterator_base &operator--() {
      pointer_ = pointer_->prev;
      return *this;
    }

    iterator_base operator--(int) {
      iterator_base temp = *this;
      pointer_ = pointer_->prev;
      return temp;
    }

    bool operator==(const iterator_base &other) const {
      return pointer_ == other.pointer_;
    }

    bool operator!=(const iterator_base &other) const {
      return pointer_ != other.pointer_;
    }

    pointer_t base() const { return pointer_; }

  private:
    pointer_t pointer_;
  };

  template <typename data_type, typename pointer_t>
  struct reverse_iterator_base {
  public:
    reverse_iterator_base() {}
    explicit reverse_iterator_base(pointer_t ptr) : pointer_(ptr) {}
    reverse_iterator_base(const reverse_iterator_base &other)
        : pointer_(other.pointer_) {}

    reverse_iterator_base &operator=(const reverse_iterator_base &other) {
      pointer_ = other.pointer_;
      return *this;
    }

    data_type &operator*() const { return pointer_->data; }
    data_type *operator->() const { return &(pointer_->data); }

    reverse_iterator_base &operator++() {
      pointer_ = pointer_->prev;
      return *this;
    }

    reverse_iterator_base operator++(int) {
      reverse_iterator_base temp = *this;
      pointer_ = pointer_->prev;
      return temp;
    }

    reverse_iterator_base &operator--() {
      pointer_ = pointer_->next;
      return *this;
    }

    reverse_iterator_base operator--(int) {
      reverse_iterator_base temp = *this;
      pointer_ = pointer_->next;
      return temp;
    }

    bool operator==(const reverse_iterator_base &other) const {
      return pointer_ == other.pointer_;
    }

    bool operator!=(const reverse_iterator_base &other) const {
      return pointer_ != other.pointer_;
    }

    pointer_t base() { return pointer_; }

  private:
    pointer_t pointer_;
  };

public:
  using iterator = iterator_base<value_type, Node *>;
  using const_iterator = iterator_base<const value_type, const Node *>;
  using reverse_iterator = reverse_iterator_base<value_type, Node *>;
  using const_reverse_iterator =
      reverse_iterator_base<const value_type, const Node *>;
  using node_type = Node;
  using Allocator = Memory::Pool<node_type>;

  List() = default;
  explicit List(const std::shared_ptr<Allocator> &pool) : m_Pool(pool) {}
  ~List() { clear(); }

  List(const List &other) = delete;
  List &operator=(const List &other) = delete;
  List(List &&other) = delete;
  List &operator=(List &&other) = delete;

  iterator begin() { return iterator(m_Head); }
  iterator end() { return iterator(nullptr); }
  const_iterator begin() const { return const_iterator(m_Head); }
  const_iterator end() const { return const_iterator(nullptr); }

  reverse_iterator rbegin() { return reverse_iterator(m_Tail); }
  reverse_iterator rend() { return reverse_iterator(nullptr); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(m_Tail);
  }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(nullptr);
  }

  template <typename T> iterator push_front(T &&data);
  template <typename T> iterator push_back(T &&data);

  void pop_front();
  void pop_back();
  void erase(const iterator &data);
  void clear();

  value_type &front() const;
  value_type &back() const;
  bool empty() const { return m_Size == 0; }

private:
  Node *m_Head = nullptr, *m_Tail = nullptr;
  size_t m_Size = 0;
  std::shared_ptr<Allocator> m_Pool{nullptr};
};
} // namespace ob::data
