#pragma once

#include <iterator>
#include <utility>

namespace archie {
namespace utils {
  namespace containers {
    template <typename Iterator>
    struct iterator_range {
      using iterator = Iterator;
      using difference_type = typename std::iterator_traits<iterator>::difference_type;
      using reference = typename std::iterator_traits<iterator>::reference;

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

      iterator begin() const noexcept;
      iterator end() const noexcept;
      template <typename Tp>
      iterator_range& set_begin(Tp) noexcept;
      template <typename Tp>
      iterator_range& set_end(Tp) noexcept;
      difference_type size() const noexcept;
      iterator_range& advance_begin(difference_type) noexcept;
      iterator_range& advance_end(difference_type) noexcept;
      reference operator[](difference_type) const noexcept;
      bool empty() const noexcept;
      explicit operator bool() const noexcept;
      template <typename Tp>
      bool in(Tp) const noexcept;
      void swap(iterator_range&) noexcept;

    private:
      iterator start_ = iterator{};
      iterator finish_ = iterator{};
    };

    template <typename Iterator>
    template <typename Tp>
    iterator_range<Iterator>::iterator_range(Tp first, Tp last) noexcept : start_(first),
                                                                           finish_(last) {}

    template <typename Iterator>
    template <typename Tp>
    iterator_range<Iterator>::iterator_range(Tp first, difference_type last) noexcept
        : start_(first),
          finish_(std::next(first, last)) {}

    template <typename Iterator>
    template <typename Tp>
    iterator_range<Iterator>& iterator_range<Iterator>::operator=(
        iterator_range<Tp> const& rhs) noexcept {
      set_begin(rhs.begin());
      set_end(rhs.end());
    }

    template <typename Iterator>
    Iterator iterator_range<Iterator>::begin() const noexcept {
      return start_;
    }

    template <typename Iterator>
    Iterator iterator_range<Iterator>::end() const noexcept {
      return finish_;
    }

    template <typename Iterator>
    template <typename Tp>
    iterator_range<Iterator>& iterator_range<Iterator>::set_begin(Tp n) noexcept {
      start_ = n;
      return *this;
    }

    template <typename Iterator>
    template <typename Tp>
    iterator_range<Iterator>& iterator_range<Iterator>::set_end(Tp n) noexcept {
      finish_ = n;
      return *this;
    }

    template <typename Iterator>
    typename iterator_range<Iterator>::difference_type iterator_range<Iterator>::size() const
        noexcept {
      return std::distance(begin(), end());
    }

    template <typename Iterator>
    iterator_range<Iterator>& iterator_range<Iterator>::advance_begin(difference_type n) noexcept {
      std::advance(start_, n);
      return *this;
    }

    template <typename Iterator>
    iterator_range<Iterator>& iterator_range<Iterator>::advance_end(difference_type n) noexcept {
      std::advance(finish_, n);
      return *this;
    }

    template <typename Iterator>
    typename iterator_range<Iterator>::reference iterator_range<Iterator>::operator[](
        difference_type n) const noexcept {
      return *std::next(begin(), n);
    }

    template <typename Iterator>
    bool iterator_range<Iterator>::empty() const noexcept {
      return begin() == end();
    }

    template <typename Iterator>
    iterator_range<Iterator>::operator bool() const noexcept {
      return !empty();
    }

    template <typename Iterator>
    template <typename Tp>
    bool iterator_range<Iterator>::in(Tp t) const noexcept {
      return (begin() <= t) && (t < end());
    }

    template <typename Iterator>
    void iterator_range<Iterator>::swap(iterator_range<Iterator>& o) noexcept {
      using std::swap;
      swap(start_, o.start_);
      swap(finish_, o.finish_);
    }
  }
}
}
