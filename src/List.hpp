#pragma once

namespace ob {
namespace data {

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
    void clear() { next = prev = nullptr; }
  };

  template <typename pointer_t> struct iterator_base {
  public:
    iterator_base() : pointer_(nullptr) {}
    explicit iterator_base(pointer_t ptr) : pointer_(ptr) {}
    iterator_base(const iterator_base &other) : pointer_(other.pointer_) {}

    iterator_base &operator=(const iterator_base &other) {
      pointer_ = other.pointer_;
      return *this;
    }

    value_type &operator*() const { return pointer_->data; }
    value_type *operator->() const { return &(pointer_->data); }

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

  template <typename pointer_t> struct reverse_iterator_base {
  public:
    reverse_iterator_base() {}
    explicit reverse_iterator_base(pointer_t ptr) : pointer_(ptr) {}
    reverse_iterator_base(const reverse_iterator_base &other)
        : pointer_(other.pointer_) {}

    reverse_iterator_base &operator=(const reverse_iterator_base &other) {
      pointer_ = other.pointer_;
      return *this;
    }

    value_type &operator*() const { return pointer_->data; }
    value_type *operator->() const { return &(pointer_->data); }

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
  using iterator = iterator_base<Node *>;
  using const_iterator = iterator_base<const Node *>;
  using reverse_iterator = reverse_iterator_base<Node *>;
  using const_reverse_iterator = reverse_iterator_base<const Node *>;

  List() {}
  ~List() { clear(); }

  iterator begin() { return iterator(head_); }
  iterator end() { return iterator(tail_); }
  const_iterator begin() const { return const_iterator(head_); }
  const_iterator end() const { return const_iterator(tail_); }

  reverse_iterator rbegin() { return reverse_iterator(tail_); }
  reverse_iterator rend() { return reverse_iterator(head_); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(tail_);
  }
  const_reverse_iterator rend() const { return const_reverse_iterator(head_); }

  template <typename T> iterator push_front(T &&data);
  template <typename T> iterator push_back(T &&data);

  void pop_front();
  void pop_back();
  void erase(const iterator &data);
  void clear();

  value_type &front() const;
  value_type &back() const;
  bool empty() const { return size_ == 0; }

private:
  Node *head_ = nullptr, *tail_ = nullptr;
  size_t size_ = 0;
};
} // namespace data
} // namespace ob