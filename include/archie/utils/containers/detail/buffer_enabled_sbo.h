#pragma once

#include <archie/utils/containers/tags.h>
#include <archie/utils/containers/types.h>
#include <archie/utils/containers/detail/buffer.h>
#include <archie/utils/containers/iterator_range.h>

namespace archie {
namespace utils {
  namespace containers {
    namespace detail {
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

        void swap(buffer& x) noexcept;
      };

      template <typename Alloc, std::size_t Stock>
      void buffer<containers::enable_sbo, Alloc, Stock>::swap(buffer& x) noexcept {
        move_assing(x);
        std::swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(x));
      }
    }
  }
}
}
