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
        using pointer = Pointer<allocator_type>;
        using const_pointer = ConstPointer<allocator_type>;
        using value_type = ValueType<allocator_type>;
        using size_type = SizeType<allocator_type>;
        using difference_type = DifferenceType<allocator_type>;
        using range_type = iterator_range<pointer>;

        using range_type::begin;
        using range_type::end;
        using range_type::empty;

        using allocator_type::allocate;
        using allocator_type::deallocate;
        using allocator_type::construct;
        using allocator_type::destroy;

        buffer() noexcept;
        explicit buffer(allocator_type const&) noexcept;
        // buffer(buffer&&) noexcept;
        // buffer(buffer const&) noexcept;
        buffer& operator=(buffer&&);
        // buffer& operator=(buffer const&);
        //~buffer() noexcept;

        RangeType<buffer> stock() const noexcept;
        RangeType<buffer>& range() noexcept;
        RangeType<buffer> const& range() const noexcept;

        SizeType<buffer> size() const noexcept;
        SizeType<buffer> capacity() const noexcept;

        bool is_on_stack() const noexcept;

        bool full() const noexcept;

        void create_storage(size_type);
        void destroy_storage() noexcept;
        void acquire(pointer, size_type, size_type) noexcept;
        Pointer<buffer> release();

        void swap(buffer& x) noexcept;

      private:
        Pointer<buffer> end_of_storage() noexcept;
        ConstPointer<buffer> end_of_storage() const noexcept;

        void clear();
        void erase();
        void move_stack(buffer&);

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
        } variant_;
      };

      template <typename Alloc, std::size_t Stock>
      RangeType<buffer<enable_sbo, Alloc, Stock>>&
      buffer<enable_sbo, Alloc, Stock>::range() noexcept {
        return static_cast<range_type&>(*this);
      }

      template <typename Alloc, std::size_t Stock>
      RangeType<buffer<enable_sbo, Alloc, Stock>> const& buffer<enable_sbo, Alloc, Stock>::range()
          const noexcept {
        return static_cast<range_type const&>(*this);
      }

      template <typename Alloc, std::size_t Stock>
      RangeType<buffer<enable_sbo, Alloc, Stock>> buffer<enable_sbo, Alloc, Stock>::stock() const
          noexcept {
        return iterator_range<pointer>(pointer(&variant_.stack[0u]), stack_size);
      }

      template <typename Alloc, std::size_t Stock>
      bool buffer<enable_sbo, Alloc, Stock>::is_on_stack() const noexcept {
        return begin() == stock().begin();
      }

      template <typename Alloc, std::size_t Stock>
      Pointer<buffer<enable_sbo, Alloc, Stock>>
      buffer<enable_sbo, Alloc, Stock>::end_of_storage() noexcept {
        return is_on_stack() ? stock().end() : variant_.heap.end_of_storage_;
      }

      template <typename Alloc, std::size_t Stock>
      ConstPointer<buffer<enable_sbo, Alloc, Stock>>
      buffer<enable_sbo, Alloc, Stock>::end_of_storage() const noexcept {
        return is_on_stack() ? stock().end() : variant_.heap.end_of_storage_;
      }

      template <typename Alloc, std::size_t Stock>
      void buffer<enable_sbo, Alloc, Stock>::clear() {
        range() = range_type(stock().begin(), 0);
      }

      template <typename Alloc, std::size_t Stock>
      void buffer<enable_sbo, Alloc, Stock>::erase() {
        while (!empty()) { destroy(range().advance_end(-1).end()); }
        destroy_storage();
        clear();
      }

      template <typename Alloc, std::size_t Stock>
      void buffer<enable_sbo, Alloc, Stock>::move_stack(buffer& x) {
        auto size = std::min(this->size(), x.size());
        auto src = x.begin();
        auto dst = begin();
        for (auto idx = 0u; idx < size; ++idx, ++src, ++dst) (*src) = std::move(*dst);
        for (; src != x.end(); ++src, ++dst) { construct(dst, std::move(*src)); }
        while (end() > dst) { destroy(range().advance_end(-1).end()); }
        range() = range_type(begin(), dst);
      }

      template <typename Alloc, std::size_t Stock>
      buffer<enable_sbo, Alloc, Stock>::buffer() noexcept
          : iterator_range<pointer>(stock().begin(), 0) {}

      template <typename Alloc, std::size_t Stock>
      buffer<enable_sbo, Alloc, Stock>::buffer(allocator_type const& a) noexcept
          : iterator_range<pointer>(stock().begin(), 0),
            Alloc(a) {}

      template <typename Alloc, std::size_t Stock>
      buffer<enable_sbo, Alloc, Stock>& buffer<enable_sbo, Alloc, Stock>::operator=(buffer&& x) {
        if (!is_on_stack() || !x.is_on_stack()) erase();
        if (x.is_on_stack()) { move_stack(x); } else {
          auto s = x.end() - x.begin();
          auto c = x.end_of_storage() - x.begin();
          acquire(x.release(), s, c);
        }
        return *this;
      }

      template <typename Alloc, std::size_t Stock>
      SizeType<buffer<enable_sbo, Alloc, Stock>> buffer<enable_sbo, Alloc, Stock>::size() const
          noexcept {
        return size_type(range().size());
      }

      template <typename Alloc, std::size_t Stock>
      SizeType<buffer<enable_sbo, Alloc, Stock>> buffer<enable_sbo, Alloc, Stock>::capacity() const
          noexcept {
        return size_type(end_of_storage() - begin());
      }

      template <typename Alloc, std::size_t Stock>
      void buffer<enable_sbo, Alloc, Stock>::create_storage(size_type n) {
        if (n > size_type(stock().size())) {
          range() = range_type(allocate(n), 0);
          variant_.heap.end_of_storage_ = begin() + n;
        } else { range() = range_type(stock().begin(), 0); }
      }

      template <typename Alloc, std::size_t Stock>
      void buffer<enable_sbo, Alloc, Stock>::destroy_storage() noexcept {
        if (!is_on_stack()) { deallocate(begin(), capacity()); }
        clear();
      }

      template <typename Alloc, std::size_t Stock>
      void buffer<enable_sbo, Alloc, Stock>::acquire(pointer ptr,
                                                     size_type s,
                                                     size_type c) noexcept {
        destroy_storage();
        range() = range_type(ptr, s);
        variant_.heap.end_of_storage_ = ptr + c;
      }

      template <typename Alloc, std::size_t Stock>
      Pointer<buffer<enable_sbo, Alloc, Stock>> buffer<enable_sbo, Alloc, Stock>::release() {
        pointer ret = nullptr;
        if (is_on_stack()) {
          ret = allocate(capacity());
          auto dest = ret;
          for (auto& val : *this) construct(dest++, std::move(val));
          while (!empty()) destroy(range().advance_end(-1).end());
        } else { ret = begin(); }
        clear();
        return ret;
      }

      template <typename Alloc, std::size_t Stock>
      void buffer<enable_sbo, Alloc, Stock>::swap(buffer& x) noexcept {
        // FIXME
        *this = std::move(x);
        std::swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(x));
      }
    }
  }
}
}
