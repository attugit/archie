#pragma once

#include <memory>
#include <type_traits>
#include <archie/utils/containers/tags.h>
#include <archie/utils/containers/iterator_range.h>

namespace archie {
namespace utils {
  namespace containers {
    namespace detail {
      template <typename, typename, std::size_t = 0u>
      struct buffer;

      template <typename Alloc>
      struct buffer<containers::disable_sbo, Alloc> : iterator_range<typename Alloc::pointer>,
                                                      Alloc {
        using allocator_type = Alloc;
        using pointer = typename allocator_type::pointer;
        using const_pointer = typename allocator_type::const_pointer;
        using size_type = typename allocator_type::size_type;

        using allocator_type::allocate;
        using allocator_type::deallocate;
        using allocator_type::construct;
        using allocator_type::destroy;

        iterator_range<pointer> stock() const noexcept;

      private:
        iterator_range<pointer>& range() noexcept;
        iterator_range<pointer> const& range() const noexcept;

        pointer end_of_storage_ = nullptr;

      public:
        void reset(pointer, pointer, pointer) noexcept;
        void reset(pointer, size_type, size_type) noexcept;
        void clear() noexcept;
        bool is_on_heap() const noexcept;

        buffer() noexcept = default;
        explicit buffer(allocator_type const&) noexcept;

        void create_storage(size_type);

        void acquire(pointer ptr, size_type s, size_type c) noexcept {
          destroy_storage();
          reset(ptr, s, c);
        }

        size_type capacity() const noexcept { return size_type(end_of_storage_ - range().begin()); }

        bool full() const noexcept { return range().end() == end_of_storage_; }

        pointer release() noexcept {
          auto ret = range().begin();
          clear();
          return ret;
        }

        void destroy_storage() noexcept {
          if (is_on_heap()) deallocate(range().begin(), capacity());
          clear();
        }

        void swap(buffer& x) noexcept {
          using std::swap;
          swap(static_cast<iterator_range<pointer>&>(*this),
               static_cast<iterator_range<pointer>&>(x));
          swap(end_of_storage_, x.end_of_storage_);
          swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(x));
        }
      };

      template <typename Alloc>
      iterator_range<typename buffer<containers::disable_sbo, Alloc>::pointer>
      buffer<containers::disable_sbo, Alloc>::stock() const noexcept {
        return iterator_range<pointer>(nullptr, nullptr);
      }

      template <typename Alloc>
      iterator_range<typename buffer<containers::disable_sbo, Alloc>::pointer>&
      buffer<containers::disable_sbo, Alloc>::range() noexcept {
        return static_cast<iterator_range<pointer>&>(*this);
      }

      template <typename Alloc>
      iterator_range<typename buffer<containers::disable_sbo, Alloc>::pointer> const&
      buffer<containers::disable_sbo, Alloc>::range() const noexcept {
        return static_cast<iterator_range<pointer> const&>(*this);
      }

      template <typename Alloc>
      void buffer<containers::disable_sbo, Alloc>::reset(pointer a, pointer b, pointer c) noexcept {
        range() = iterator_range<pointer>(a, b);
        end_of_storage_ = c;
      }

      template <typename Alloc>
      void buffer<containers::disable_sbo, Alloc>::reset(pointer a,
                                                         size_type s,
                                                         size_type c) noexcept {
        reset(a, a + s, a + c);
      }

      template <typename Alloc>
      void buffer<containers::disable_sbo, Alloc>::clear() noexcept {
        reset(nullptr, nullptr, nullptr);
      }

      template <typename Alloc>
      bool buffer<containers::disable_sbo, Alloc>::is_on_heap() const noexcept {
        return range().begin() != nullptr;
      }

      template <typename Alloc>
      buffer<containers::disable_sbo, Alloc>::buffer(allocator_type const& a) noexcept
          : allocator_type(a) {
        clear();
      }

      template <typename Alloc>
      void buffer<containers::disable_sbo, Alloc>::create_storage(size_type n) {
        if (n != capacity()) acquire((n != 0u) ? allocate(n) : nullptr, 0u, n);
      }

      template <typename Alloc, std::size_t Stock>
      struct buffer<containers::enable_sbo, Alloc, Stock> : iterator_range<typename Alloc::pointer>,
                                                            Alloc {
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
        iterator_range<pointer>& range() noexcept {
          return static_cast<iterator_range<pointer>&>(*this);
        };
        iterator_range<pointer> const& range() const noexcept {
          return static_cast<iterator_range<pointer> const&>(*this);
        };

        struct heap_data {
          heap_data() = default;
          pointer end_of_storage_ = nullptr;
        };

        static constexpr auto const stack_size = Stock > (sizeof(heap_data) / sizeof(value_type))
                                                     ? Stock
                                                     : (sizeof(heap_data) / sizeof(value_type));
        static_assert(stack_size > 0u, "");

      private:
        union variant {
          variant() : heap() {}
          ~variant() {}
          heap_data heap;
          value_type stack[stack_size];
        };

        variant variant_;

      public:
        iterator_range<pointer> stock() const noexcept {
          return iterator_range<pointer>(pointer(&variant_.stack[0u]), stack_size);
        }

      private:
        bool is_on_stack() const noexcept { return range().begin() == stock().begin(); }

        pointer end_of_storage() noexcept {
          return is_on_stack() ? stock().end() : variant_.heap.end_of_storage_;
        }

        const_pointer end_of_storage() const noexcept {
          return is_on_stack() ? stock().end() : variant_.heap.end_of_storage_;
        }

        void clear() {
          range().set_begin(stock().begin());
          range().set_end(stock().begin());
        }

        size_type size() const noexcept { return size_type(range().size()); }

        void erase_all() {
          while (!range().empty()) { destroy(range().advance_end(-1).end()); }
          clear();
        }

        void move_stack(buffer& x) {
          auto size = std::min(size_type(range().size()), size_type(x.size()));
          auto src = x.begin();
          auto dst = range().begin();
          for (auto idx = 0u; idx < size; ++idx, ++src, ++dst) (*src) = std::move(*dst);
          while (src != x.end()) {
            construct(dst, std::move(*src));
            ++src;
            ++dst;
          }
          while (range().end() > dst) { destroy(range().advance_end(-1).end()); }
          range().set_end(dst);
        }

      public:
        void move_assing(buffer& x) {
          if (!is_on_stack() || !x.is_on_stack()) erase_all();
          if (x.is_on_stack()) { move_stack(x); } else {
            auto s = x.end() - x.begin();
            auto c = x.end_of_storage() - x.begin();
            acquire(x.release(), s, c);
          }
        }

        buffer() noexcept : iterator_range<pointer>(stock().begin(), 0) {}

        size_type capacity() const noexcept {
          return size_type(end_of_storage() - range().begin());
        }

        bool full() const noexcept { return range().end() == end_of_storage(); }

        void create_storage(size_type n) {
          if (n > size_type(stock().size())) {
            range().set_begin(allocate(n));
            variant_.heap.end_of_storage_ = range().begin() + n;
          } else { range().set_begin(stock().begin()); }
          range().set_end(range().begin());
        }

        void acquire(pointer ptr, size_type s, size_type c) noexcept {
          destroy_storage();
          range().set_begin(ptr);
          range().set_end(ptr + s);
          variant_.heap.end_of_storage_ = ptr + c;
        }

        void destroy_storage() noexcept {
          if (!is_on_stack()) { deallocate(range().begin(), capacity()); }
          clear();
        }

        pointer release() noexcept {
          pointer ret = nullptr;
          if (is_on_stack()) {
            ret = allocate(capacity());
            auto dest = ret;
            for (auto& val : *this) construct(dest++, std::move(val));
            while (!range().empty()) destroy(range().advance_end(-1).end());
          } else { ret = range().begin(); }
          clear();
          return ret;
        }

        void swap(buffer& x) noexcept {
          move_assing(x);
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
