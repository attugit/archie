#pragma once

#include <memory>
#include <type_traits>
#include <archie/utils/containers/tags.h>
#include <archie/utils/containers/types.h>
#include <archie/utils/containers/detail/buffer.h>
#include <archie/utils/containers/detail/buffer_disabled_sbo.h>
#include <archie/utils/containers/detail/buffer_enabled_sbo.h>

namespace archie {
namespace utils {
  namespace containers {
    template <typename Tp,
              typename Alloc = std::allocator<Tp>,
              typename Tag = disable_sbo,
              std::size_t Stock = 0u>
    struct dynamic_array {
      using allocator_type = Alloc;
      using pointer = typename allocator_type::pointer;
      using const_pointer = typename allocator_type::const_pointer;
      using value_type = typename allocator_type::value_type;
      using reference = typename allocator_type::reference;
      using const_reference = typename allocator_type::const_reference;
      using size_type = typename allocator_type::size_type;
      using difference_type = typename allocator_type::difference_type;

      using iterator = __gnu_cxx::__normal_iterator<pointer, dynamic_array>;
      using const_iterator = __gnu_cxx::__normal_iterator<const_pointer, dynamic_array>;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
      using impl_t = detail::buffer<Tag, allocator_type, Stock>;

      impl_t impl_;

      void realloc(size_type n) {
        if (capacity() != n) {
          impl_.destroy_storage();
          impl_.create_storage(n);
        }
      }

      template <typename Iter>
      void reset(Iter first, Iter last, size_type addon = 0u) {
        clear();
        realloc(std::distance(first, last) + addon);
        for (; first != last; ++first) emplace_back(*first);
      }

    public:
      dynamic_array() = default;

      explicit dynamic_array(size_type n) { impl_.create_storage(n); }

      dynamic_array(size_type n, allocator_type const& a) : impl_(a) { impl_.create_storage(n); }

      dynamic_array(dynamic_array&& x) noexcept { impl_.swap(x.impl_); }

      dynamic_array& operator=(dynamic_array&& x) noexcept {
        clear();
        realloc(0u);
        impl_.swap(x.impl_);
        return *this;
      }

      dynamic_array(dynamic_array const& x) : dynamic_array(x.capacity()) {
        std::copy(std::begin(x), std::end(x), std::back_inserter(*this));
      }

      dynamic_array& operator=(dynamic_array const& x) {
        reset(std::begin(x), std::end(x), x.capacity() - x.size());
        return *this;
      }

      dynamic_array(std::initializer_list<value_type> list)
          : dynamic_array(std::begin(list), std::end(list)) {}

      dynamic_array& operator=(std::initializer_list<value_type> list) {
        reset(std::begin(list), std::end(list));
        return *this;
      }

      template <typename InputIterator>
      dynamic_array(InputIterator first, InputIterator last)
          : dynamic_array(std::distance(first, last)) {
        reset(first, last);
      }

      ~dynamic_array() noexcept {
        clear();
        impl_.destroy_storage();
      }

      size_type capacity() const noexcept { return impl_.capacity(); }

      size_type size() const noexcept { return size_type(impl_.end() - impl_.begin()); }

      constexpr size_type stock() const noexcept { return impl_.stock(); }

      bool empty() const noexcept { return cbegin() == cend(); }

      bool full() const noexcept { return impl_.full(); }

      template <typename... Args>
      void emplace_back(Args&&... args) noexcept(
          std::is_nothrow_constructible<value_type, Args...>::value) {
        impl_.construct(impl_.end(), std::forward<Args>(args)...);
        impl_.advance_end(1);
      }

      void push_back(const_reference x) noexcept(
          std::is_nothrow_copy_constructible<value_type>::value) {
        emplace_back(x);
      }

      void push_back(value_type&& x) noexcept(
          std::is_nothrow_move_constructible<value_type>::value) {
        emplace_back(std::move(x));
      }

      void pop_back() noexcept { impl_.destroy(impl_.advance_end(-1).end()); }

      void erase(iterator pos) noexcept(std::is_nothrow_move_assignable<value_type>::value) {
        std::move(pos + 1, end(), pos);
        pop_back();
      }

      void clear() noexcept {
        while (!empty()) pop_back();
      }

      const_reference operator[](size_type n) const noexcept { return *(impl_.begin() + n); }

      reference operator[](size_type n) noexcept { return *(impl_.begin() + n); }

      iterator begin() noexcept { return iterator(impl_.begin()); }

      iterator end() noexcept { return iterator(impl_.end()); }

      const_iterator cbegin() const noexcept { return const_iterator(impl_.begin()); }

      const_iterator begin() const noexcept { return cbegin(); }

      const_iterator cend() const noexcept { return const_iterator(impl_.end()); }

      const_iterator end() const noexcept { return cend(); }

      pointer data() noexcept { return impl_.begin(); }

      const_pointer data() const noexcept { return impl_.begin(); }

      void acquire(pointer first, pointer last) noexcept { acquire(first, last - first); }

      void acquire(pointer ptr, size_type s) noexcept { acquire(ptr, s, s); }

      void acquire(pointer ptr, size_type s, size_type c) noexcept {
        clear();
        impl_.acquire(ptr, s, c);
      }

      void acquire(allocator_type alloc, pointer ptr, size_type s, size_type c) noexcept {
        acquire(ptr, s, c);
        get_allocator() = std::move(alloc);
      }

      pointer release() noexcept(noexcept(std::declval<impl_t>().release())) {
        return impl_.release();
      }

      allocator_type& get_allocator() noexcept { return static_cast<allocator_type&>(impl_); }

      allocator_type const& get_allocator() const noexcept {
        return static_cast<allocator_type const&>(impl_);
      }
    };
  }
}
}
