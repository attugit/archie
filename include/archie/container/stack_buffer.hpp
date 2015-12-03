#pragma once
#include <initializer_list>
#include <archie/container/base_buffer.hpp>

namespace archie {
template <typename T, std::size_t N>
struct stack_buffer : base_buffer<stack_buffer<T, N>> {
private:
  using base_t = base_buffer<stack_buffer<T, N>>;
  using traits = array_traits<stack_buffer<T, N>>;

public:
  using value_type = typename traits::value_type;
  using pointer = typename traits::pointer;
  using const_pointer = typename traits::const_pointer;
  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;
  using size_type = typename traits::size_type;
  using difference_type = typename traits::difference_type;
  using iterator = typename traits::iterator;
  using const_iterator = typename traits::const_iterator;

private:
  union storage {
    storage() {}
    ~storage() {}
    char dummy;
    value_type data[N];
  } store;

public:
  stack_buffer() = default;
  stack_buffer(std::initializer_list<value_type> init) : base_t() {
    for (auto const& x : init) this->emplace_back(x);
  }
  stack_buffer(stack_buffer const& other) : base_t() {
    for (auto const& x : other) this->emplace_back(x);
  }
  stack_buffer(stack_buffer&& other) : base_t() {
    for (auto& x : other) this->emplace_back(std::move(x));
  }
  stack_buffer& operator=(stack_buffer const& other) {
    this->assign([](const_reference r) -> const_reference { return r; }, this->begin(),
                 other.begin(), other.end());
    return *this;
  }
  stack_buffer& operator=(stack_buffer&& other) {
    this->assign([](reference r) -> value_type&& { return std::move(r); }, this->begin(),
                 other.begin(), other.end());
    return *this;
  }
  ~stack_buffer() { this->clear(); }

  pointer data() { return &store.data[0]; }
  const_pointer data() const { return &store.data[0]; }
  size_type capacity() const { return N; }
};

template <typename T, std::size_t N>
struct array_traits<stack_buffer<T, N>> : base_factory<T> {
  using value_type = T;
  using pointer = value_type*;
  using const_pointer = value_type const*;
  using reference = value_type&;
  using const_reference = value_type const&;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
};
}
