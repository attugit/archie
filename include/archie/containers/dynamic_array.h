#pragma once

#include <memory>
#include <type_traits>
#include <archie/containers/tags.h>
#include <archie/containers/types.h>
#include <archie/containers/detail/buffer.h>
#include <archie/containers/detail/buffer_disabled_sbo.h>
#include <archie/containers/detail/buffer_enabled_sbo.h>

namespace archie {
namespace containers {
  template <typename Tp,
            typename Alloc = std::allocator<Tp>,
            typename Tag = disable_sbo,
            std::size_t Stock = 0u>
  struct dynamic_array {
    using allocator_type = Alloc;
    using pointer = Pointer<allocator_type>;
    using const_pointer = ConstPointer<allocator_type>;
    using value_type = ValueType<allocator_type>;
    using reference = Reference<allocator_type>;
    using const_reference = ConstReference<allocator_type>;
    using size_type = SizeType<allocator_type>;
    using difference_type = DifferenceType<allocator_type>;

    using iterator = __gnu_cxx::__normal_iterator<pointer, dynamic_array>;
    using const_iterator = __gnu_cxx::__normal_iterator<const_pointer, dynamic_array>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  private:
    using impl_t = detail::buffer<Tag, allocator_type, Stock>;
    impl_t impl_;

    struct is_nothrow {
      static constexpr bool copy_constructible =
          std::is_nothrow_copy_constructible<value_type>::value;
      static constexpr bool move_constructible =
          std::is_nothrow_move_constructible<value_type>::value;
      static constexpr bool move_assignable = std::is_nothrow_move_assignable<value_type>::value;
    };

  public:
    dynamic_array() noexcept = default;
    explicit dynamic_array(size_type);
    dynamic_array(size_type, allocator_type const&);
    dynamic_array(dynamic_array const&) noexcept(is_nothrow::copy_constructible);
    dynamic_array(dynamic_array&&) noexcept(is_nothrow::move_constructible);
    dynamic_array(std::initializer_list<value_type>) noexcept(is_nothrow::copy_constructible);
    dynamic_array& operator=(dynamic_array const&) noexcept(is_nothrow::copy_constructible);
    dynamic_array& operator=(dynamic_array&&) noexcept(is_nothrow::move_constructible);
    dynamic_array& operator=(std::initializer_list<value_type>) noexcept(
        is_nothrow::copy_constructible);
    template <typename InputIterator>
    dynamic_array(InputIterator, InputIterator) noexcept(
        std::is_nothrow_constructible<value_type, decltype(*std::declval<InputIterator>())>::value);
    ~dynamic_array() noexcept;

    SizeType<dynamic_array> capacity() const noexcept;
    SizeType<dynamic_array> size() const noexcept;
    RangeType<impl_t> stock() const noexcept;
    bool empty() const noexcept;

    template <typename... Args>
    void emplace_back(Args&&...) noexcept(
        std::is_nothrow_constructible<value_type, Args...>::value);
    void push_back(const_reference) noexcept(is_nothrow::copy_constructible);
    void push_back(value_type&&) noexcept(is_nothrow::move_constructible);

    void pop_back() noexcept;
    void erase(iterator) noexcept(is_nothrow::move_assignable);
    void clear() noexcept;

    ConstReference<dynamic_array> operator[](size_type) const noexcept;
    Reference<dynamic_array> operator[](size_type) noexcept;

    Iterator<dynamic_array> begin() noexcept;
    Iterator<dynamic_array> end() noexcept;
    ConstIterator<dynamic_array> cbegin() const noexcept;
    ConstIterator<dynamic_array> begin() const noexcept;
    ConstIterator<dynamic_array> cend() const noexcept;
    ConstIterator<dynamic_array> end() const noexcept;
    Pointer<dynamic_array> data() noexcept;
    ConstPointer<dynamic_array> data() const noexcept;

    void acquire(pointer, pointer) noexcept;
    void acquire(dynamic_array&) noexcept;
    void acquire(pointer, size_type) noexcept;
    void acquire(pointer, size_type, size_type) noexcept;
    void acquire(allocator_type, pointer, size_type, size_type) noexcept;

    Pointer<dynamic_array> release() noexcept(noexcept(std::declval<impl_t>().release()));

    Allocator<dynamic_array>& get_allocator() noexcept;
    Allocator<dynamic_array> const& get_allocator() const noexcept;

  private:
    template <typename Iter>
    void reset(Iter, Iter, size_type = 0u);
  };

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>::dynamic_array(size_type n) {
    impl_.create_storage(n);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>::dynamic_array(size_type n, allocator_type const& a)
      : impl_(a) {
    impl_.create_storage(n);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>::dynamic_array(dynamic_array&& x) noexcept(
      is_nothrow::move_constructible)
      : impl_(std::move(x.impl_)) {}

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>& dynamic_array<Tp, Alloc, Tag, Stock>::operator=(
      dynamic_array&& x) noexcept(is_nothrow::move_constructible) {
    clear();
    impl_ = std::move(x.impl_);
    return *this;
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>::dynamic_array(dynamic_array const& x) noexcept(
      is_nothrow::copy_constructible)
      : dynamic_array(x.capacity()) {
    for (auto const& e : x) emplace_back(e);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>& dynamic_array<Tp, Alloc, Tag, Stock>::operator=(
      dynamic_array const& x) noexcept(is_nothrow::copy_constructible) {
    reset(std::begin(x), std::end(x), x.capacity() - x.size());
    return *this;
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>::dynamic_array(
      std::initializer_list<value_type> list) noexcept(is_nothrow::copy_constructible)
      : dynamic_array(std::begin(list), std::end(list)) {}

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>& dynamic_array<Tp, Alloc, Tag, Stock>::operator=(
      std::initializer_list<value_type> list) noexcept(is_nothrow::copy_constructible) {
    reset(std::begin(list), std::end(list));
    return *this;
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  template <typename InputIterator>
  dynamic_array<Tp, Alloc, Tag, Stock>::
      dynamic_array(InputIterator first, InputIterator last) noexcept(
          std::is_nothrow_constructible<value_type,
                                        decltype(*std::declval<InputIterator>())>::value)
      : dynamic_array(std::distance(first, last)) {
    reset(first, last);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  dynamic_array<Tp, Alloc, Tag, Stock>::~dynamic_array() noexcept {
    clear();
    impl_.destroy_storage();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  SizeType<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::capacity()
      const noexcept {
    return impl_.capacity();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  SizeType<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::size() const
      noexcept {
    return impl_.size();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  RangeType<typename dynamic_array<Tp, Alloc, Tag, Stock>::impl_t>
  dynamic_array<Tp, Alloc, Tag, Stock>::stock() const noexcept {
    return impl_.stock();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  bool dynamic_array<Tp, Alloc, Tag, Stock>::empty() const noexcept {
    return begin() == end();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  template <typename... Args>
  void dynamic_array<Tp, Alloc, Tag, Stock>::emplace_back(Args&&... args) noexcept(
      std::is_nothrow_constructible<value_type, Args...>::value) {
    impl_.construct(impl_.end(), std::forward<Args>(args)...);
    impl_.advance_end(1);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::push_back(const_reference x) noexcept(
      is_nothrow::copy_constructible) {
    emplace_back(x);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::push_back(value_type&& x) noexcept(
      is_nothrow::move_constructible) {
    emplace_back(std::move(x));
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::pop_back() noexcept {
    impl_.destroy(impl_.advance_end(-1).end());
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::erase(iterator pos) noexcept(
      is_nothrow::move_assignable) {
    std::move(pos + 1, end(), pos);
    pop_back();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::clear() noexcept {
    while (!empty()) pop_back();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  ConstReference<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::
  operator[](size_type n) const noexcept {
    return *(data() + n);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  Reference<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::operator[](
      size_type n) noexcept {
    return *(data() + n);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  Iterator<dynamic_array<Tp, Alloc, Tag, Stock>>
  dynamic_array<Tp, Alloc, Tag, Stock>::end() noexcept {
    return iterator(impl_.end());
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  Iterator<dynamic_array<Tp, Alloc, Tag, Stock>>
  dynamic_array<Tp, Alloc, Tag, Stock>::begin() noexcept {
    return iterator(data());
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  ConstIterator<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::cbegin()
      const noexcept {
    return const_iterator(data());
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  ConstIterator<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::begin()
      const noexcept {
    return cbegin();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  ConstIterator<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::cend()
      const noexcept {
    return const_iterator(impl_.end());
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  ConstIterator<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::end()
      const noexcept {
    return cend();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  Pointer<dynamic_array<Tp, Alloc, Tag, Stock>>
  dynamic_array<Tp, Alloc, Tag, Stock>::data() noexcept {
    return impl_.begin();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  ConstPointer<dynamic_array<Tp, Alloc, Tag, Stock>> dynamic_array<Tp, Alloc, Tag, Stock>::data()
      const noexcept {
    return impl_.begin();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::acquire(pointer first, pointer last) noexcept {
    acquire(first, last - first);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::acquire(dynamic_array& x) noexcept {
    auto s = x.size();
    auto c = x.capacity();
    acquire(x.release(), s, c);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::acquire(pointer ptr, size_type s) noexcept {
    acquire(ptr, s, s);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::acquire(pointer ptr,
                                                     size_type s,
                                                     size_type c) noexcept {
    clear();
    impl_.acquire(ptr, s, c);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  void dynamic_array<Tp, Alloc, Tag, Stock>::acquire(allocator_type alloc,
                                                     pointer ptr,
                                                     size_type s,
                                                     size_type c) noexcept {
    acquire(ptr, s, c);
    get_allocator() = std::move(alloc);
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  Pointer<dynamic_array<Tp, Alloc, Tag, Stock>>
  dynamic_array<Tp, Alloc, Tag, Stock>::release() noexcept(
      noexcept(std::declval<dynamic_array<Tp, Alloc, Tag, Stock>::impl_t>().release())) {
    return impl_.release();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  Allocator<dynamic_array<Tp, Alloc, Tag, Stock>>&
  dynamic_array<Tp, Alloc, Tag, Stock>::get_allocator() noexcept {
    return impl_.get_allocator();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  Allocator<dynamic_array<Tp, Alloc, Tag, Stock>> const&
  dynamic_array<Tp, Alloc, Tag, Stock>::get_allocator() const noexcept {
    return impl_.get_allocator();
  }

  template <typename Tp, typename Alloc, typename Tag, std::size_t Stock>
  template <typename Iter>
  void dynamic_array<Tp, Alloc, Tag, Stock>::reset(Iter first, Iter last, size_type addon) {
    clear();
    impl_.resize(std::distance(first, last) + addon);
    for (; first != last; ++first) emplace_back(*first);
  }
}
}
