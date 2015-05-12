#pragma once

#include <archie/utils/containers/tags.h>
#include <archie/utils/containers/types.h>
#include <archie/utils/containers/detail/buffer.h>
#include <archie/utils/containers/iterator_range.h>

namespace archie {
namespace utils {
  namespace containers {
    namespace detail {
      template <typename Alloc>
      struct buffer<containers::disable_sbo, Alloc> : iterator_range<Pointer<Alloc>>, Alloc {
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

        buffer() noexcept = default;
        explicit buffer(allocator_type const&) noexcept;
        buffer(buffer&&) noexcept;
        // buffer(buffer const&) noexcept;
        buffer& operator=(buffer&&) noexcept;
        // buffer& operator=(buffer const&);
        //~buffer() noexcept;

        RangeType<buffer> stock() const noexcept;
        RangeType<buffer>& range() noexcept;
        RangeType<buffer> const& range() const noexcept;

        size_type size() const noexcept;
        size_type capacity() const noexcept;

        void reset(pointer, pointer, pointer) noexcept;
        void reset(pointer, size_type, size_type) noexcept;
        void clear() noexcept;
        bool is_on_heap() const noexcept;

        void create_storage(size_type);
        void destroy_storage() noexcept;
        void acquire(pointer, size_type, size_type) noexcept;
        pointer release() noexcept;
        void swap(buffer&) noexcept;

      private:
        pointer end_of_storage_ = nullptr;
      };

      template <typename Alloc>
      buffer<containers::disable_sbo, Alloc>::buffer(buffer&& x) noexcept : buffer() {
        swap(x);
      }

      template <typename Alloc>
      buffer<containers::disable_sbo, Alloc>& buffer<containers::disable_sbo, Alloc>::operator=(
          buffer&& x) noexcept {
        auto s = x.size();
        auto c = x.capacity();
        acquire(x.release(), s, c);
        static_cast<allocator_type&>(*this) = std::move(static_cast<allocator_type&>(x));
        return *this;
      }

      template <typename Alloc>
      RangeType<buffer<containers::disable_sbo, Alloc>>
      buffer<containers::disable_sbo, Alloc>::stock() const noexcept {
        return range_type(nullptr, nullptr);
      }

      template <typename Alloc>
      RangeType<buffer<containers::disable_sbo, Alloc>>&
      buffer<containers::disable_sbo, Alloc>::range() noexcept {
        return static_cast<range_type&>(*this);
      }

      template <typename Alloc>
      RangeType<buffer<containers::disable_sbo, Alloc>> const&
      buffer<containers::disable_sbo, Alloc>::range() const noexcept {
        return static_cast<range_type const&>(*this);
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
        return begin() != nullptr;
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

      template <typename Alloc>
      void buffer<containers::disable_sbo, Alloc>::destroy_storage() noexcept {
        if (is_on_heap()) deallocate(begin(), capacity());
        clear();
      }

      template <typename Alloc>
      void buffer<containers::disable_sbo, Alloc>::acquire(pointer ptr,
                                                           size_type s,
                                                           size_type c) noexcept {
        destroy_storage();
        reset(ptr, s, c);
      }

      template <typename Alloc>
      void buffer<containers::disable_sbo, Alloc>::swap(buffer& x) noexcept {
        using std::swap;
        swap(range(), x.range());
        swap(end_of_storage_, x.end_of_storage_);
        swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(x));
      }

      template <typename Alloc>
      typename buffer<containers::disable_sbo, Alloc>::pointer
      buffer<containers::disable_sbo, Alloc>::release() noexcept {
        auto ret = begin();
        clear();
        return ret;
      }

      template <typename Alloc>
      typename buffer<containers::disable_sbo, Alloc>::size_type
      buffer<containers::disable_sbo, Alloc>::size() const noexcept {
        return size_type(range().size());
      }

      template <typename Alloc>
      typename buffer<containers::disable_sbo, Alloc>::size_type
      buffer<containers::disable_sbo, Alloc>::capacity() const noexcept {
        return size_type(end_of_storage_ - begin());
      }
    }
  }
}
}
