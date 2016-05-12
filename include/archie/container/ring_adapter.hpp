#pragma once
#include <utility>
#include <archie/container/ring_iterator.hpp>
#include <archie/traits/model_of.hpp>

namespace archie {
template <typename Container>
struct ring_adapter {
private:
  struct is_valid_container {
    template <typename C>
    auto requires(C) -> decltype(std::declval<C const>().begin(),
                                 std::declval<C const>().end(),
                                 std::declval<C const>().size(),
                                 std::declval<C const>().capacity());
  };
  struct can_emplace {
    template <typename C, typename... Args>
    auto requires(C, Args...) -> decltype(std::declval<C>().emplace_back(std::declval<Args>()...),
                                          typename C::value_type{std::declval<Args>()...});
  };
  static_assert(traits::model_of<is_valid_container(Container)>::value == true, "");

public:
  using value_type = typename Container::value_type;
  using size_type = typename Container::size_type;
  using difference_type = typename Container::difference_type;
  using iterator = ring_iterator<typename Container::iterator>;
  using const_iterator = ring_iterator<typename Container::const_iterator>;

  template <typename... Args>
  explicit ring_adapter(Args&&... args)
      : container_(std::forward<Args>(args)...), pos_(container_, 0)
  {
  }

  iterator begin() { return pos_; }
  const_iterator begin() const { return iterator{container_, 0}; }
  iterator end() { return begin() + size(); }
  const_iterator end() const { return begin() + size(); }
  size_type size() const { return container_.size(); }
  size_type capacity() const { return container_.capacity(); }
  bool empty() const { return size() == 0; }
  template <typename... Args>
  void emplace_back(Args&&... args)
  {
    static_assert(traits::model_of<can_emplace(Container, Args...)>::value, "");
    if (size() != capacity()) {
      container_.emplace_back(std::forward<Args>(args)...);
      pos_ = iterator{container_, 0};
    }
    else
    {
      *pos_++ = std::move(value_type{std::forward<Args>(args)...});
    }
  }

  Container* operator->() { return &container_; }
  Container const* operator->() const { return &container_; }
  Container& operator*() { return container_; }
  Container const& operator*() const { return container_; }
private:
  Container container_;
  iterator pos_;
};
}
