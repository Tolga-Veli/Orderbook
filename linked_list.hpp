
namespace Tools {

template <typename T> class LinkedList {
public:
  struct node {
    T data;
    node *next = nullptr, *prev = nullptr;

    node() : data() {}
    explicit node(T _data) : data(_data) {}
  };

  LinkedList() {
    root = new node();
    last = root;
  }

  ~LinkedList() { clear(); }

  node *insert_back(T data);
  void pop_back();
  void remove(node *data);
  void clear();

private:
  node *root, *last;
}; // namespace Tools
} // namespace Tools