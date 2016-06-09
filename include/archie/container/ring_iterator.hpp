#pragma once
#include <iterator>
#include <algorithm>
#include <boost/iterator/iterator_facade.hpp>

namespace archie {

template <typename Iterator>
struct ring_iterator
    : boost::iterator_facade<ring_iterator<Iterator>,
                             std::remove_reference_t<decltype(*std::declval<Iterator>())>,
                             boost::random_access_traversal_tag> {
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;

private:
  friend class boost::iterator_core_access;
  template <typename>
  friend struct ring_iterator;

  Iterator front_;
  difference_type to_last_ = 0;
  difference_type offset_ = 0;

  Iterator normalize() const { return std::next(front_, offset_ % to_last_); }
  void advance(difference_type n) { offset_ += n; }
  void increment() { advance(1); }
  void decrement() { advance(-1); }
  template <typename Iter>
  bool equal(ring_iterator<Iter> const& rhs) const
  {
    return (offset_ == rhs.offset_) && normalize() == rhs.normalize();
  }

  template <typename Iter>
  auto distance_to(ring_iterator<Iter> const& rhs) const
  {
    return rhs.offset_ - offset_;
  }

  auto& dereference() const { return *normalize(); }
  struct enabler {
  };

public:
  template <typename Iter, typename Distance0, typename Distance1>
  explicit ring_iterator(Iter it, Distance0 last, Distance1 offset)
      : front_(it),
        to_last_(last != 0 ? static_cast<difference_type>(last) : 1),
        offset_(static_cast<difference_type>(offset))
  {
  }

  template <typename Iter, typename Distance>
  explicit ring_iterator(Iter begin, Iter last, Distance offset)
      : ring_iterator(begin, std::distance(begin, last), offset)
  {
  }

  template <typename Iter>
  explicit ring_iterator(Iter begin, Iter last) : ring_iterator(begin, last, 0)
  {
  }

  template <typename Iter>
  explicit ring_iterator(Iter begin, Iter last, Iter first)
      : ring_iterator(begin, last, std::distance(begin, first))
  {
  }

  template <typename Cont>
  explicit ring_iterator(Cont& cont, typename Cont::difference_type offset)
      : ring_iterator(std::begin(cont), cont.size(), offset)
  {
  }

  template <typename Iter>
  ring_iterator(ring_iterator<Iter> const& other,
                typename std::enable_if<std::is_convertible<Iter, Iterator>::value, enabler>::type =
                    enabler{})
      : ring_iterator(other.front_, other.to_last_, other.offset_)
  {
  }

  ring_iterator() : ring_iterator(Iterator{}, 0, 0) {}
};
}
