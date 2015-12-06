#pragma once

#include <archie/containers/tags.h>
#include <archie/containers/types.h>
#include <archie/containers/detail/buffer.h>
#include <archie/containers/iterator_range.h>

namespace archie {
namespace containers {
  namespace detail {
    template <typename Alloc>
    struct buffer<disable_sbo, Alloc> : iterator_range<Pointer<Alloc>>, Alloc {
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

      bool is_on_heap() const noexcept;
      Allocator<buffer> const& get_allocator() const noexcept;
      Allocator<buffer>& get_allocator() noexcept;

      void create_storage(size_type);
      void destroy_storage() noexcept;
      void acquire(pointer, size_type, size_type) noexcept;
      Pointer<buffer> release() noexcept;
      void clear() noexcept;
      void resize(size_type);

    private:
      using allocator_type::allocate;
      using allocator_type::deallocate;

      pointer end_of_storage_ = nullptr;
    };

    template <typename Alloc>
    buffer<disable_sbo, Alloc>::buffer(buffer&& x) noexcept : buffer() {
      range() = x.range();
      end_of_storage_ = x.end_of_storage_;
      x.clear();
      get_allocator() = std::move(x.get_allocator());
    }

    template <typename Alloc>
    buffer<disable_sbo, Alloc>& buffer<disable_sbo, Alloc>::operator=(buffer&& x) noexcept {
      auto s = x.size();
      auto c = x.capacity();
      acquire(x.release(), s, c);
      get_allocator() = std::move(x.get_allocator());
      return *this;
    }

    template <typename Alloc>
    RangeType<buffer<disable_sbo, Alloc>> buffer<disable_sbo, Alloc>::stock() const noexcept {
      return range_type(nullptr, nullptr);
    }

    template <typename Alloc>
    Allocator<buffer<disable_sbo, Alloc>>& buffer<disable_sbo, Alloc>::get_allocator() noexcept {
      return static_cast<allocator_type&>(*this);
    }

    template <typename Alloc>
    Allocator<buffer<disable_sbo, Alloc>> const& buffer<disable_sbo, Alloc>::get_allocator() const
        noexcept {
      return static_cast<allocator_type const&>(*this);
    }

    template <typename Alloc>
    RangeType<buffer<disable_sbo, Alloc>>& buffer<disable_sbo, Alloc>::range() noexcept {
      return static_cast<range_type&>(*this);
    }

    template <typename Alloc>
    RangeType<buffer<disable_sbo, Alloc>> const& buffer<disable_sbo, Alloc>::range() const
        noexcept {
      return static_cast<range_type const&>(*this);
    }

    template <typename Alloc>
    void buffer<disable_sbo, Alloc>::clear() noexcept {
      range() = range_type(nullptr, nullptr);
      end_of_storage_ = begin();
    }

    template <typename Alloc>
    void buffer<disable_sbo, Alloc>::resize(size_type n) {
      if (n != capacity()) {
        destroy_storage();
        create_storage(n);
      }
    }

    template <typename Alloc>
    bool buffer<disable_sbo, Alloc>::is_on_heap() const noexcept {
      return begin() != nullptr;
    }

    template <typename Alloc>
    buffer<disable_sbo, Alloc>::buffer(allocator_type const& a) noexcept : allocator_type(a) {
      clear();
    }

    template <typename Alloc>
    void buffer<disable_sbo, Alloc>::create_storage(size_type n) {
      if (n != 0) {
        range() = range_type(allocate(n), 0u);
        end_of_storage_ = begin() + n;
      }
    }

    template <typename Alloc>
    void buffer<disable_sbo, Alloc>::destroy_storage() noexcept {
      if (is_on_heap()) deallocate(begin(), capacity());
      clear();
    }

    template <typename Alloc>
    void buffer<disable_sbo, Alloc>::acquire(pointer ptr, size_type s, size_type c) noexcept {
      destroy_storage();
      range() = range_type(ptr, s);
      end_of_storage_ = begin() + c;
    }

    template <typename Alloc>
    Pointer<buffer<disable_sbo, Alloc>> buffer<disable_sbo, Alloc>::release() noexcept {
      auto ret = begin();
      clear();
      return ret;
    }

    template <typename Alloc>
    typename buffer<disable_sbo, Alloc>::size_type buffer<disable_sbo, Alloc>::size() const
        noexcept {
      return size_type(range().size());
    }

    template <typename Alloc>
    typename buffer<disable_sbo, Alloc>::size_type buffer<disable_sbo, Alloc>::capacity() const
        noexcept {
      return size_type(end_of_storage_ - begin());
    }
  }
}
}
