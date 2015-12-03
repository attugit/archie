#pragma once
#include <memory>
#include <initializer_list>
#include <archie/container/base_buffer.hpp>

namespace archie {
namespace detail {
  template <typename Buffer, std::size_t N, typename Alloc>
  struct storage_t : Alloc {
    using size_type = typename Buffer::size_type;
    using pointer = typename Buffer::pointer;
    using const_pointer = typename Buffer::const_pointer;
    using value_type = typename Buffer::value_type;

  private:
    pointer data_ = nullptr;
    union u {
      u() {}
      ~u() {}
      size_type capacity_;
      value_type stack_[N];
    } u_;

  public:
    storage_t() : data_(&(u_.stack_[0])) {}
    explicit storage_t(size_type S) : data_(S > N ? Alloc::allocate(S) : &(u_.stack_[0])) {
      if (is_on_heap()) u_.capacity_ = S;
    }
    storage_t(storage_t&& orig) : storage_t() { *this = std::move(orig); }
    storage_t& operator=(storage_t&& orig) {
      if (orig.is_on_heap()) {
        using std::swap;
        swap(data_, orig.data_);
        swap(u_.capacity_, orig.u_.capacity_);
        orig.data_ = &(orig.u_.stack_[0]);
      }
      return *this;
    }
    ~storage_t() {
      if (is_on_heap()) Alloc::deallocate(data_, capacity());
    }

    bool is_on_heap() const { return data_ != &(u_.stack_[0]); }
    pointer data() { return data_; }
    const_pointer data() const { return data_; }
    size_type capacity() const { return is_on_heap() ? u_.capacity_ : N; }
    void realloc(size_type S) {
      if (is_on_heap()) Alloc::deallocate(data_, capacity());
      data_ = S > N ? Alloc::allocate(S) : &(u_.stack_[0]);
      if (is_on_heap()) u_.capacity_ = S;
    }
  };

  template <typename Buffer, typename Alloc>
  struct storage_t<Buffer, 0, Alloc> : Alloc {
    using size_type = typename Buffer::size_type;
    using pointer = typename Buffer::pointer;
    using const_pointer = typename Buffer::const_pointer;

    storage_t() : storage_t(0) {}
    explicit storage_t(size_type S)
        : data_(S > 0 ? Alloc::allocate(S) : nullptr), capacity_(data_ != nullptr ? S : 0) {}
    storage_t(storage_t&& orig) : storage_t(0) { *this = std::move(orig); }
    storage_t& operator=(storage_t&& orig) {
      using std::swap;
      swap(data_, orig.data_);
      swap(capacity_, orig.capacity_);
      return *this;
    }
    ~storage_t() {
      if (data() != nullptr && capacity() > 0) Alloc::deallocate(data(), capacity());
    }

    constexpr bool is_on_heap() const { return true; }
    pointer data() { return data_; }
    const_pointer data() const { return data_; }
    size_type capacity() const { return capacity_; }
    void realloc(size_type S) {
      if (data() != nullptr && capacity() > 0) Alloc::deallocate(data(), capacity());
      data_ = S > 0 ? Alloc::allocate(S) : nullptr;
      capacity_ = data_ != nullptr ? S : 0;
    }

  private:
    pointer data_ = nullptr;
    size_type capacity_ = 0;
  };
}

template <typename T, std::size_t N, typename Alloc = std::allocator<T>>
struct mixed_buffer : base_buffer<mixed_buffer<T, N, Alloc>> {
private:
  using base_t = base_buffer<mixed_buffer<T, N, Alloc>>;
  using traits = array_traits<mixed_buffer<T, N, Alloc>>;

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
  using storage_t = detail::storage_t<mixed_buffer<T, N, Alloc>, N, Alloc>;
  storage_t storage_;

  void realloc(size_type S) {
    this->clear();
    storage_.realloc(S);
    this->reset();
  }

public:
  mixed_buffer() : base_t() { this->reset(); }
  explicit mixed_buffer(size_type S) : base_t(nullptr), storage_(S) { this->reset(); }
  mixed_buffer(std::initializer_list<value_type> init) : mixed_buffer(init.size()) {
    for (auto const& x : init) this->emplace_back(x);
  }
  mixed_buffer(mixed_buffer const& orig) : mixed_buffer(orig.size()) {
    for (auto const& x : orig) this->emplace_back(x);
  }
  mixed_buffer(mixed_buffer&& orig) : mixed_buffer() { *this = std::move(orig); }
  mixed_buffer& operator=(mixed_buffer const& orig) {
    if (this->capacity() != orig.capacity()) this->realloc(orig.capacity());
    this->assign([](const_reference r) -> const_reference { return r; }, this->begin(),
                 orig.begin(), orig.end());
    return *this;
  }
  mixed_buffer& operator=(mixed_buffer&& orig) {
    this->realloc(0);
    if (orig.storage_.is_on_heap()) {
      using std::swap;
      swap(this->storage_, orig.storage_);
      swap(this->end_, orig.end_);
      orig.reset();
    } else {
      for (auto& x : orig) this->emplace_back(std::move(x));
    }
    return *this;
  }
  ~mixed_buffer() { this->clear(); }

  pointer data() { return this->storage_.data(); }
  const_pointer data() const { return this->storage_.data(); }
  size_type capacity() const { return this->storage_.capacity(); }
};

template <typename T, std::size_t N, typename Alloc>
struct array_traits<mixed_buffer<T, N, Alloc>>
    : base_factory<typename Alloc::value_type, typename Alloc::pointer> {
  using value_type = typename Alloc::value_type;
  using pointer = typename Alloc::pointer;
  using const_pointer = typename Alloc::const_pointer;
  using reference = typename Alloc::reference;
  using const_reference = typename Alloc::const_reference;
  using size_type = typename Alloc::size_type;
  using difference_type = typename Alloc::difference_type;
  using iterator = pointer;
  using const_iterator = const_pointer;
};

template <typename T, typename Alloc = std::allocator<T>>
using heap_buffer = mixed_buffer<T, 0, Alloc>;
}
