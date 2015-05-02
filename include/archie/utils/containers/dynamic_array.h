#pragma once

#include <memory>
#include <type_traits>
#include <archie/utils/containers/tags.h>

namespace archie {
namespace utils {
  namespace containers {
    namespace detail {
      template <typename, typename, std::size_t = 0u>
      struct buffer;

      template <typename Alloc>
      struct buffer<containers::disable_sbo, Alloc> : Alloc {
        using allocator_type = Alloc;
        using pointer = typename allocator_type::pointer;
        using const_pointer = typename allocator_type::const_pointer;
        using size_type = typename allocator_type::size_type;

        using allocator_type::allocate;
        using allocator_type::deallocate;
        using allocator_type::construct;
        using allocator_type::destroy;

        constexpr size_type stock() const noexcept { return 0u; }

      private:
        pointer start_ = nullptr;
        pointer finish_ = nullptr;
        pointer end_of_storage_ = nullptr;

        void reset(pointer a, size_type s, size_type c) noexcept { reset(a, a + s, a + c); }

        void reset(pointer a, pointer b, pointer c) noexcept {
          start_ = a;
          finish_ = b;
          end_of_storage_ = c;
        }

        void clear() { reset(nullptr, nullptr, nullptr); }

        bool is_on_heap() const noexcept { return begin() != nullptr; }

      public:
        buffer() noexcept = default;

        explicit buffer(allocator_type const& a) noexcept : allocator_type(a) { clear(); }

        void create_storage(size_type n) {
          if (n != capacity()) acquire((n != 0u) ? allocate(n) : nullptr, 0u, n);
        }

        void acquire(pointer ptr, size_type s, size_type c) noexcept {
          destroy_storage();
          reset(ptr, s, c);
        }

        pointer begin() noexcept { return start_; }

        const_pointer begin() const noexcept { return start_; }

        pointer end() noexcept { return finish_; }

        const_pointer end() const noexcept { return finish_; }

        pointer advance_end(int n) noexcept {
          finish_ += n;
          return end();
        }

        size_type capacity() const noexcept { return size_type(end_of_storage_ - start_); }

        bool full() const noexcept { return finish_ == end_of_storage_; }

        pointer release() noexcept {
          auto ret = begin();
          clear();
          return ret;
        }

        void destroy_storage() noexcept {
          if (is_on_heap()) deallocate(begin(), capacity());
          clear();
        }

        void swap(buffer& x) noexcept {
          std::swap(start_, x.start_);
          std::swap(finish_, x.finish_);
          std::swap(end_of_storage_, x.end_of_storage_);
          std::swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(x));
        }
      };

      template <typename Alloc>
      struct buffer<containers::enable_sbo, Alloc> : Alloc {
        using allocator_type = Alloc;
        using pointer = typename allocator_type::pointer;
        using const_pointer = typename allocator_type::const_pointer;
        using value_type = typename allocator_type::value_type;
        using size_type = typename allocator_type::size_type;
        using difference_type = typename allocator_type::difference_type;

        using allocator_type::allocate;
        using allocator_type::deallocate;
        using allocator_type::construct;
        using allocator_type::destroy;

      private:
        struct heap_data {
          heap_data() = default;
          pointer start_ = nullptr;
          pointer end_of_storage_ = nullptr;
        };

        static constexpr auto const stack_size = sizeof(heap_data) / sizeof(value_type);
        static_assert(stack_size > 0u, "");

      public:
        constexpr size_type stock() const noexcept { return stack_size; }

      private:
        union variant {
          variant() : heap() {}
          ~variant() {}
          heap_data heap;
          value_type stack[stack_size];
        };

        pointer finish_ = nullptr;
        variant variant_;

        pointer stack_begin() noexcept { return pointer(&variant_.stack[0]); }

        const_pointer stack_begin() const noexcept { return pointer(&variant_.stack[0]); }

        pointer stack_end() noexcept { return pointer(&variant_.stack[stock()]); }

        const_pointer stack_end() const noexcept { return const_pointer(&variant_.stack[stock()]); }

        bool is_on_stack() const noexcept { return end() >= stack_begin() && end() <= stack_end(); }

        pointer end_of_storage() noexcept {
          return is_on_stack() ? stack_end() : variant_.heap.end_of_storage_;
        }

        const_pointer end_of_storage() const noexcept {
          return is_on_stack() ? stack_end() : variant_.heap.end_of_storage_;
        }

        void clear() noexcept { finish_ = stack_begin(); }

      public:
        buffer() noexcept : finish_(stack_begin()) {}

        pointer begin() noexcept { return is_on_stack() ? stack_begin() : variant_.heap.start_; }

        const_pointer begin() const noexcept {
          return is_on_stack() ? stack_begin() : variant_.heap.start_;
        }

        pointer end() noexcept { return finish_; }

        const_pointer end() const noexcept { return finish_; }

        pointer advance_end(int n) noexcept {
          finish_ += n;
          return finish_;
        }

        size_type capacity() const noexcept { return size_type(end_of_storage() - begin()); }

        bool full() const noexcept { return end() == end_of_storage(); }

        void create_storage(size_type n) {
          if (n > stock()) {
            variant_.heap.start_ = allocate(n);
            finish_ = variant_.heap.start_;
            variant_.heap.end_of_storage_ = finish_ + n;
          } else { clear(); }
        }

        void acquire(pointer a, pointer b, pointer c) noexcept {
          destroy_storage();
          variant_.heap.start_ = a;
          finish_ = b;
          variant_.heap.end_of_storage_ = c;
        }

        void acquire(pointer ptr, size_type s, size_type c) noexcept {
          acquire(ptr, ptr + s, ptr + c);
        }

        void destroy_storage() noexcept {
          if (!is_on_stack()) { deallocate(begin(), capacity()); }
          clear();
        }

        pointer release() {
          pointer ret = nullptr;
          if (is_on_stack()) {
            ret = allocate(capacity());
            auto dest = ret;
            for (auto& val : *this) construct(dest++, std::move(val));
            while (end() != begin()) destroy(advance_end(-1));
          } else { ret = begin(); }
          clear();
          return ret;
        }

        void move_in(buffer&& x) noexcept /* condition */ {
          if (x.is_on_stack()) {
            destroy_storage();
            for (auto it = x.begin(); it != x.end(); ++it) {
              construct(finish_++, std::move(*it));
              it->~value_type();
            }
          } else { acquire(x.begin(), x.end(), x.end_of_storage()); }
          x.clear();
        }

        void swap(buffer& x) noexcept {
          move_in(std::move(x));
          std::swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(x));
        }
      };

      template <typename Alloc, std::size_t Stock>
      struct buffer<containers::enable_ra_sbo, Alloc, Stock> : Alloc {
        using allocator_type = Alloc;
        using pointer = typename allocator_type::pointer;
        using const_pointer = typename allocator_type::const_pointer;
        using value_type = typename allocator_type::value_type;
        using size_type = typename allocator_type::size_type;
        using difference_type = typename allocator_type::difference_type;

        using allocator_type::allocate;
        using allocator_type::deallocate;
        using allocator_type::construct;
        using allocator_type::destroy;

      private:
        struct heap_data {
          heap_data() = default;
          pointer end_of_storage_ = nullptr;
        };

        static constexpr auto const stack_size = Stock > (sizeof(heap_data) / sizeof(value_type))
                                                     ? Stock
                                                     : (sizeof(heap_data) / sizeof(value_type));
        static_assert(stack_size > 0u, "");

      public:
        constexpr size_type stock() const noexcept { return stack_size; }

      private:
        union variant {
          variant() : heap() {}
          ~variant() {}
          heap_data heap;
          value_type stack[stack_size];
        };

        pointer start_ = nullptr;
        pointer finish_ = nullptr;
        variant variant_;

        pointer stack_begin() noexcept { return pointer(&variant_.stack[0]); }

        const_pointer stack_begin() const noexcept { return pointer(&variant_.stack[0]); }

        pointer stack_end() noexcept { return pointer(&variant_.stack[stock()]); }

        const_pointer stack_end() const noexcept { return const_pointer(&variant_.stack[stock()]); }

        bool is_on_stack() const noexcept { return begin() == stack_begin(); }

        pointer end_of_storage() noexcept {
          return is_on_stack() ? stack_end() : variant_.heap.end_of_storage_;
        }

        const_pointer end_of_storage() const noexcept {
          return is_on_stack() ? stack_end() : variant_.heap.end_of_storage_;
        }

        void clear() {
          start_ = stack_begin();
          finish_ = start_;
        }

      public:
        buffer() noexcept : start_(stack_begin()), finish_(stack_begin()) {}

        pointer begin() noexcept { return start_; }

        const_pointer begin() const noexcept { return start_; }

        pointer end() noexcept { return finish_; }

        const_pointer end() const noexcept { return finish_; }

        pointer advance_end(int n) noexcept {
          finish_ += n;
          return finish_;
        }

        size_type capacity() const noexcept { return size_type(end_of_storage() - begin()); }

        bool full() const noexcept { return end() == end_of_storage(); }

        void create_storage(size_type n) {
          if (n > stock()) {
            start_ = allocate(n);
            variant_.heap.end_of_storage_ = start_ + n;
          } else { start_ = stack_begin(); }
          finish_ = start_;
        }

        void acquire(pointer ptr, size_type s, size_type c) noexcept {
          destroy_storage();
          start_ = ptr;
          finish_ = start_ + s;
          variant_.heap.end_of_storage_ = start_ + c;
        }

        void destroy_storage() noexcept {
          if (!is_on_stack()) { deallocate(begin(), capacity()); }
          clear();
        }

        pointer release() noexcept {
          pointer ret = nullptr;
          if (is_on_stack()) {
            ret = allocate(capacity());
            auto dest = ret;
            for (auto& val : *this) construct(dest++, std::move(val));
            while (end() != begin()) destroy(advance_end(-1));
          } else { ret = begin(); }
          clear();
          return ret;
        }

        void swap(buffer& x) noexcept {
          if (x.is_on_stack()) {
            std::move(x.begin(), x.end(), begin());
            start_ = stack_begin();
            finish_ = start_ + size_type(x.end() - x.begin());
          } else {
            start_ = x.begin();
            finish_ = x.end();
            variant_.heap.end_of_storage_ = x.end_of_storage();
            x.clear();
          }
          std::swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(x));
        }
      };
    }

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

      void pop_back() noexcept { impl_.destroy(impl_.advance_end(-1)); }

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
