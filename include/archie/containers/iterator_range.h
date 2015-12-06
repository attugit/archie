#pragma once

#include <iterator>
#include <utility>
#include <archie/containers/types.h>

namespace archie {
namespace containers {
  template <typename Iter>
  struct iterator_range {
    using iterator = Iter;
    using difference_type = DifferenceType<std::iterator_traits<iterator>>;
    using reference = Reference<std::iterator_traits<iterator>>;

    iterator_range() noexcept = default;
    template <typename Tp>
    iterator_range(Tp, Tp) noexcept;
    template <typename Tp>
    iterator_range(Tp, difference_type) noexcept;
    iterator_range(iterator_range const&) noexcept = default;
    iterator_range(iterator_range&&) noexcept = default;
    iterator_range& operator=(iterator_range const&) noexcept = default;
    iterator_range& operator=(iterator_range&&) noexcept = default;

    template <typename Tp>
    iterator_range& operator=(iterator_range<Tp> const&) noexcept;

    Iterator<iterator_range> begin() const noexcept;
    Iterator<iterator_range> end() const noexcept;
    DifferenceType<iterator_range> size() const noexcept;
    iterator_range& advance_begin(difference_type) noexcept;
    iterator_range& advance_end(difference_type) noexcept;
    Reference<iterator_range> operator[](difference_type) const noexcept;
    bool empty() const noexcept;
    explicit operator bool() const noexcept;
    template <typename Tp>
    bool contains(Tp) const noexcept;
    void swap(iterator_range&) noexcept;

  private:
    iterator start_ = iterator{};
    iterator finish_ = iterator{};
  };

  template <typename Iter>
  template <typename Tp>
  iterator_range<Iter>::iterator_range(Tp first, Tp last) noexcept : start_(first), finish_(last) {}

  template <typename Iter>
  template <typename Tp>
  iterator_range<Iter>::iterator_range(Tp first, difference_type last) noexcept
      : start_(first),
        finish_(std::next(first, last)) {}

  template <typename Iter>
  template <typename Tp>
  iterator_range<Iter>& iterator_range<Iter>::operator=(iterator_range<Tp> const& rhs) noexcept {
    start_ = rhs.begin();
    finish_ = rhs.end();
  }

  template <typename Iter>
  Iterator<iterator_range<Iter>> iterator_range<Iter>::begin() const noexcept {
    return start_;
  }

  template <typename Iter>
  Iterator<iterator_range<Iter>> iterator_range<Iter>::end() const noexcept {
    return finish_;
  }

  template <typename Iter>
  DifferenceType<iterator_range<Iter>> iterator_range<Iter>::size() const noexcept {
    return std::distance(begin(), end());
  }

  template <typename Iter>
  iterator_range<Iter>& iterator_range<Iter>::advance_begin(difference_type n) noexcept {
    std::advance(start_, n);
    return *this;
  }

  template <typename Iter>
  iterator_range<Iter>& iterator_range<Iter>::advance_end(difference_type n) noexcept {
    std::advance(finish_, n);
    return *this;
  }

  template <typename Iter>
  Reference<iterator_range<Iter>> iterator_range<Iter>::operator[](difference_type n) const
      noexcept {
    return *std::next(begin(), n);
  }

  template <typename Iter>
  bool iterator_range<Iter>::empty() const noexcept {
    return begin() == end();
  }

  template <typename Iter>
  iterator_range<Iter>::operator bool() const noexcept {
    return !empty();
  }

  template <typename Iter>
  template <typename Tp>
  bool iterator_range<Iter>::contains(Tp t) const noexcept {
    return (begin() <= t) && (t < end());
  }

  template <typename Iter>
  void iterator_range<Iter>::swap(iterator_range<Iter>& o) noexcept {
    using std::swap;
    swap(start_, o.start_);
    swap(finish_, o.finish_);
  }
}
}
