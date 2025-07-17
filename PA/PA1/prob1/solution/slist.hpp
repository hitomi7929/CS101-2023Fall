#ifndef CS101_SLIST_HPP
#define CS101_SLIST_HPP

#include <algorithm>
#include <concepts>
#include <iterator>
#include <memory>

#include "slist_node.hpp"

template <typename T, bool IsConst> struct SlistIterator {
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = std::conditional_t<IsConst, const T &, T &>;
  using pointer = std::conditional_t<IsConst, const T *, T *>;
  using iterator_category = std::forward_iterator_tag;

private:
  using node_type = SlistNode<T>;
  using self_type = SlistIterator<T, IsConst>;

  node_type *m_node{};

  template <typename OtherDecayed>
  static constexpr auto isAddingLowLevelConst =
      IsConst && std::is_same_v<OtherDecayed, SlistIterator<T, false>>;

public:
  SlistIterator() = default;

  explicit SlistIterator(const std::unique_ptr<node_type> &up)
      : m_node{up.get()} {}

  template <typename Other>
    requires isAddingLowLevelConst<std::decay_t<Other>>
  SlistIterator(Other &&other) : m_node{other.base()} {}

  auto base() const { return m_node; }

  // TODO: IMPLEMENT THIS.
  reference operator*() const {
    return m_node->value;
  }

  pointer operator->() const { return std::addressof(operator*()); }

  // TODO: IMPLEMENT THIS.
  self_type &operator++() {
    if (m_node) {
      m_node = (m_node->next).get();
    }
    return *this;
  }

  self_type operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  // TODO: IMPLEMENT THIS.
  template <bool C> bool operator==(const SlistIterator<T, C> &other) const {
    return (m_node == other.base());
  }
};

template <typename T> class Slist {
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = SlistIterator<T, false>;
  using const_iterator = SlistIterator<T, true>;

private:
  using node_type = SlistNode<T>;
  using node_uptr = std::unique_ptr<node_type>;

  node_uptr m_head;

public:
  Slist() = default;

  // TODO: IMPLEMENT THIS.
  Slist(const Slist &other):Slist(other.begin(),other.end()) {}

  void swap(Slist &other) noexcept { m_head.swap(other.m_head); }

  Slist &operator=(const Slist &other) {
    Slist(other).swap(*this);
    return *this;
  }

  Slist(Slist &&) noexcept = default;

  Slist &operator=(Slist &&) noexcept = default;

  ~Slist() = default;

  // TODO: IMPLEMENT THIS.
  Slist(size_type count, const T &value) {
    for (size_type i = 0; i < count; i++) {
      emplace_front(std::move(value));
    }
  }

  // TODO: IMPLEMENT THIS.
  explicit Slist(size_type count) {
    m_head = std::make_unique<node_type>(std::move(m_head));
    for (size_type i = 1; i < count; i++) {
      emplace_front();
    }
  }

  // TODO: IMPLEMENT THIS.
  Slist(std::input_iterator auto b, std::input_iterator auto e) {
    if (b == e) { return; }
    emplace_front(*b);
    auto pos = begin();
    b++;
    for (; b != e; b++) {
      pos = emplace_after(pos, *b);
    }
  }

  Slist(std::initializer_list<T> init) : Slist(init.begin(), init.end()) {}

  iterator begin() { return iterator(m_head); }
  iterator end() { return {}; }
  const_iterator cbegin() const { return const_iterator(m_head); }
  const_iterator cend() const { return {}; }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }

  void push_front(const T &x) {
    emplace_front(x); // Why?
  }

  void push_front(T &&x) {
    emplace_front(std::move(x)); // Why?
  }

  // TODO: IMPLEMENT THIS.
  template <typename... Types> reference emplace_front(Types &&...args) {
    m_head = std::make_unique<node_type>(std::move(m_head), std::forward<Types>(args)...);
    return m_head->value;
  }

  // TODO: IMPLEMENT THIS.
  void pop_front() {
    if (m_head) {
      m_head = std::move(m_head->next);
    }
  }

  void clear() {
    m_head.reset(); // Why?
  }

  // TODO: IMPLEMENT THIS.
  void insert_after(const_iterator pos, const T &x) {
    emplace_after(pos, x);
  }

  // TODO: IMPLEMENT THIS.
  void insert_after(const_iterator pos, T &&x) {
    emplace_after(pos, std::move(x));
  }

  // TODO: IMPLEMENT THIS.
  template <typename... Types>
  iterator emplace_after(const_iterator pos, Types &&...args) {
    pos.base()->next = std::make_unique<node_type>(std::move(pos.base()->next), std::forward<Types>(args)...);
    return iterator(pos.base()->next);
  }

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos) {
    if (pos.base()->next != nullptr) {
      pos.base()->next = std::move(pos.base()->next->next);
      return iterator(pos.base()->next);
    }
    return end();
  }

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos, const_iterator last) {
    auto curr = pos;
    while (curr.base()->next.get() != last.base()) { curr++; }
    pos.base()->next = std::move(curr.base()->next);
    return iterator(pos.base()->next);
  }

  // TODO: IMPLEMENT THIS.
  bool empty() const {
    return m_head == nullptr;
  }

  // TODO: IMPLEMENT THIS.
  size_type size() const {
    if (this->empty()) { return 0; }
    size_type ans = 0;
    for (auto iter = this->cbegin(); iter != this->cend(); iter++) {
      ans++;
    }
    return ans;
  }

  reference front() { return m_head->value; }
  const_reference front() const { return m_head->value; }

  bool operator==(const Slist<T> &other) const {
    return std::ranges::equal(*this, other);
  }

  auto operator<=>(const Slist<T> &other) const {
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(),
                                                  other.end());
  }

  void merge(Slist<T> &other) { merge(std::move(other)); }

  void merge(Slist<T> &&other) { merge(std::move(other), std::less<>{}); }

  void merge(Slist<T> &other, auto compare) {
    merge(std::move(other), compare);
  }

  // TODO: IMPLEMENT THIS.
  void merge(Slist<T> &&other, auto compare) {
    if (other.begin() == other.end()) { return; }
    if (begin() == other.begin()) { return; }
    if (empty()) {
      swap(other);
      return;
    }
    if (compare(other.m_head->value, m_head->value)) {
      swap(other);
      (begin().base()->next).swap(other.m_head);
    }
    auto curr = begin();
    while (curr.base()->next != nullptr && other.begin() != other.end()) {
      if (compare(other.m_head->value, curr.base()->next->value)) {
        (curr.base()->next).swap(other.m_head);
        curr++;
        (curr.base()->next).swap(other.m_head);
      } else {
        curr++;
      }
    }
    if (other.begin() != other.end()) {
      (curr.base()->next).swap(other.m_head);
    }
  }

  void sort() { sort(std::less<>{}); }

  // TODO: IMPLEMENT THIS.
  void sort(auto compare) {
    if (begin() == end() || begin().base()->next == nullptr) { return; }
    size_type sublen = size() / 2;
    auto mid = begin();
    for (size_type i = 1; i < sublen; i++) {
      mid++;
    }
    auto sub = Slist();
    (sub.m_head).swap(mid.base()->next);
    sort(compare);
    sub.sort(compare);
    merge(sub, compare);
  }
};

template <std::input_iterator Iterator>
Slist(Iterator, Iterator)
    -> Slist<typename std::iterator_traits<Iterator>::value_type>;

namespace std {
template <typename T> void swap(Slist<T> &a, Slist<T> &b) noexcept {
  a.swap(b);
}
} // namespace std

#endif // CS101_SLIST_HPP