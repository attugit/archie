#pragma once
#include <algorithm>

namespace archie {
template <typename>
struct array_traits;

template <typename ValueType, typename Pointer = std::add_pointer_t<ValueType>>
struct base_factory {
  template <typename... Args>
  static void construct(Pointer p, Args&&... args) {
    new (p) ValueType{std::forward<Args>(args)...};
  }
  static void destroy(Pointer p) noexcept { p->~ValueType(); }
};

template <typename S>
struct base_buffer {
  using storage_type = S;

private:
  using traits = array_traits<storage_type>;
  storage_type& self() { return static_cast<storage_type&>(*this); }
  storage_type const& self() const { return static_cast<storage_type const&>(*this); }

public:
  base_buffer() : base_buffer(begin()) {}

  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;
  using iterator = typename traits::iterator;
  using const_iterator = typename traits::const_iterator;
  using size_type = typename traits::size_type;

  iterator begin() { return iterator{self().data()}; }
  const_iterator cbegin() const { return const_iterator{self().data()}; }
  const_iterator begin() const { return cbegin(); }

  iterator end() { return end_; }
  const_iterator cend() const { return end_; }
  const_iterator end() const { return cend(); }

  size_type max_size() const { return self().capacity(); }
  size_type size() const { return static_cast<size_type>(std::distance(cbegin(), cend())); }
  bool empty() const { return size() == 0; }

  reference operator[](size_type pos) { return *(begin() + pos); }
  const_reference operator[](size_type pos) const { return *(begin() + pos); }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    traits::construct(end_++, std::forward<Args>(args)...);
  }

  void pop_back() { traits::destroy(--end_); }

  void clear() {
    while (!empty()) pop_back();
  }

protected:
  explicit base_buffer(iterator e) : end_(e) {}
  void reset() { end_ = this->begin(); }
  template <typename F, typename Iterator>
  void assign(F f, iterator d_first, Iterator i_first, Iterator i_last) {
    if (d_first != i_first) {
      while (d_first != this->end() && i_first != i_last) { *d_first++ = f(*i_first++); }
      while (i_first != i_last) {
        this->emplace_back(f(*i_first++));
        ++d_first;
      }
      while (d_first != this->end()) { this->pop_back(); }
    }
  }

  iterator end_;
};

template <typename S>
bool operator==(base_buffer<S> const& lhs, base_buffer<S> const& rhs) {
  return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs));
}
template <typename S>
bool operator!=(base_buffer<S> const& lhs, base_buffer<S> const& rhs) {
  return !(lhs == rhs);
}
template <typename S>
bool operator<(base_buffer<S> const& lhs, base_buffer<S> const& rhs) {
  return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                      std::end(rhs));
}
}
