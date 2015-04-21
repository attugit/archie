#pragma once

#include <memory>
#include <tuple>
#include <type_traits>
#include <archie/utils/containers/tags.h>

namespace archie {
namespace utils {
  namespace containers {
    namespace detail {
      template <typename, typename>
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

      private:
        pointer start_ = nullptr;
        pointer finish_ = nullptr;
        pointer end_of_storage_ = nullptr;

        void clear() {
          start_ = nullptr;
          finish_ = nullptr;
          end_of_storage_ = nullptr;
        }

      public:
        buffer() noexcept = default;

        explicit buffer(allocator_type const& a) noexcept : allocator_type(a) { clear(); }

        void create_storage(size_type n) {
          if (n != size_type(end_of_storage_ - start_)) {
            start_ = (n != 0u) ? allocate(n) : nullptr;
            finish_ = start_;
            end_of_storage_ = start_ + n;
          }
        }

        pointer begin() noexcept { return start_; }

        const_pointer begin() const noexcept { return start_; }

        pointer end() noexcept { return finish_; }

        const_pointer end() const noexcept { return finish_; }

        pointer advance_end(int n) noexcept {
          finish_ += n;
          return finish_;
        }

        size_type capacity() const noexcept { return size_type(end_of_storage_ - start_); }

        bool full() const noexcept { return finish_ == end_of_storage_; }

        std::tuple<pointer, pointer, pointer> release() noexcept {
          auto ret = std::make_tuple(start_, finish_, end_of_storage_);
          clear();
          return ret;
        }

        void destroy_storage() noexcept {
          if (start_ != nullptr) deallocate(begin(), capacity());
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

        pointer stack_end() noexcept { return pointer(&variant_.stack[stack_size]); }

        const_pointer stack_end() const noexcept {
          return const_pointer(&variant_.stack[stack_size]);
        }

        bool is_on_stack() const noexcept { return end() >= stack_begin() && end() < stack_end(); }

        pointer end_of_storage() noexcept {
          return is_on_stack() ? stack_end() : variant_.heap.end_of_storage_;
        }

        const_pointer end_of_storage() const noexcept {
          return is_on_stack() ? stack_end() : variant_.heap.end_of_storage_;
        }

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
          if (n > stack_size) {
            variant_.heap.start_ = allocate(n);
            finish_ = variant_.heap.start_;
            variant_.heap.end_of_storage_ = finish_ + n;
          } else { finish_ = stack_begin(); }
        }

        void destroy_storage() {
          if (!is_on_stack()) { deallocate(begin(), capacity()); }
          finish_ = stack_begin();
        }

        std::tuple<pointer, pointer, pointer> release() noexcept {
          auto ret = std::make_tuple(nullptr, nullptr, nullptr);
          return ret;
        }

        void swap(buffer& x) noexcept {
          if (x.is_on_stack()) {
            std::move(x.begin(), x.end(), begin());
            finish_ = stack_begin() + size_type(x.end() - x.begin());
          } else {
            finish_ = x.end();
            variant_.heap.start_ = x.begin();
            variant_.heap.end_of_storage_ = x.end_of_storage();
            x.finish_ = x.stack_begin();
          }
          std::swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(x));
        }
      };
    }

    template <typename Tp, typename Alloc = std::allocator<Tp>, typename Tag = disable_sbo>
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
      using impl_t = detail::buffer<Tag, allocator_type>;

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
        std::copy(first, last, std::back_inserter(*this));
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

      std::tuple<pointer, pointer, pointer> release() noexcept(
          noexcept(std::declval<impl_t>().release())) {
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
