#pragma once
#include <iterator>
#include <algorithm>
#include <functional>
#include <archie/assignable_const.hpp>

namespace archie {
template <typename Iterator>
struct ring_iterator : std::iterator<typename std::iterator_traits<Iterator>::iterator_category,
                                     typename std::iterator_traits<Iterator>::value_type,
                                     typename std::iterator_traits<Iterator>::difference_type,
                                     typename std::iterator_traits<Iterator>::pointer,
                                     typename std::iterator_traits<Iterator>::reference> {
private:
  using iterator = Iterator;
  using traits = std::iterator_traits<iterator>;

public:
  using difference_type = typename traits::difference_type;
  using reference = typename traits::reference;
  using pointer = typename traits::pointer;

  ///--------------------------------------------
  template <typename Iter, typename Distance0, typename Distance1>
  explicit ring_iterator(Iter begin, Distance0 last, Distance1 offset)
      : front_(begin),
        to_last_(static_cast<difference_type>(last != 0 ? last : 1)),
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
  template <typename Range>
  explicit ring_iterator(Range& r, typename std::remove_cv_t<Range>::difference_type n)
      : ring_iterator(std::begin(r), r.size(), n)
  {
  }
  template <typename Range>
  explicit ring_iterator(Range& r) : ring_iterator(r, 0)
  {
  }
  ///--------------------------------------------
  ring_iterator() : ring_iterator(iterator{}, iterator{}) {}
  ring_iterator(ring_iterator const&) = default;
  ring_iterator(ring_iterator&&) = default;
  ring_iterator& operator=(ring_iterator const&) = default;
  ring_iterator& operator=(ring_iterator&&) = default;
  ///--------------------------------------------
  reference operator*() const { return *normalize(); }
  pointer operator->() const { return normalize(); }
  ///--------------------------------------------
  reference operator[](difference_type n) const { return *(*this + n); }
  ///--------------------------------------------
  friend ring_iterator& operator+=(ring_iterator& lhs, difference_type rhs)
  {
    lhs.offset_ += rhs;
    return lhs;
  }
  friend difference_type operator-(ring_iterator const& lhs, ring_iterator const& rhs)
  {
    return lhs.offset_ - rhs.offset_;
  }
  ///--------------------------------------------
  friend bool operator==(ring_iterator const& lhs, ring_iterator const& rhs)
  {
    return (lhs.offset_ == rhs.offset_) && lhs.normalize() == rhs.normalize();
  }
  friend bool operator<(ring_iterator const& lhs, ring_iterator const& rhs)
  {
    return lhs.offset_ < rhs.offset_;
  }
  ///--------------------------------------------
  friend void normalize(ring_iterator& it) { it.offset_ %= it.to_last_; }
private:
  iterator normalize() const { return std::next<iterator>(front_, offset_ % to_last_); }
  ///--------------------------------------------
  assignable_const<iterator> front_;
  assignable_const<difference_type> to_last_;
  difference_type offset_;
};
///--------------------------------------------
template <typename Iterator>
bool operator!=(ring_iterator<Iterator> const& lhs, ring_iterator<Iterator> const& rhs)
{
  return !(lhs == rhs);
}
template <typename Iterator>
bool operator>(ring_iterator<Iterator> const& lhs, ring_iterator<Iterator> const& rhs)
{
  return rhs < lhs;
}
template <typename Iterator>
bool operator>=(ring_iterator<Iterator> const& lhs, ring_iterator<Iterator> const& rhs)
{
  return !(lhs < rhs);
}
template <typename Iterator>
bool operator<=(ring_iterator<Iterator> const& lhs, ring_iterator<Iterator> const& rhs)
{
  return !(rhs < lhs);
}
///--------------------------------------------
template <typename Iterator, typename Distance>
ring_iterator<Iterator> operator+(ring_iterator<Iterator> const& lhs, Distance rhs)
{
  ring_iterator<Iterator> ret = lhs;
  ret += static_cast<typename ring_iterator<Iterator>::difference_type>(rhs);
  return ret;
}
template <typename Iterator, typename Distance>
ring_iterator<Iterator> operator+(Distance lhs, ring_iterator<Iterator> const& rhs)
{
  return rhs + lhs;
}
template <typename Iterator>
ring_iterator<Iterator>& operator++(ring_iterator<Iterator>& lhs)
{
  lhs += 1;
  return lhs;
}
template <typename Iterator>
ring_iterator<Iterator> operator++(ring_iterator<Iterator>& lhs, int)
{
  ring_iterator<Iterator> ret = lhs;
  ++lhs;
  return ret;
}
template <typename Iterator, typename Distance>
ring_iterator<Iterator>& operator-=(ring_iterator<Iterator>& lhs, Distance rhs)
{
  return lhs += (-rhs);
}
template <typename Iterator>
ring_iterator<Iterator>& operator--(ring_iterator<Iterator>& lhs)
{
  lhs -= 1;
  return lhs;
}
template <typename Iterator>
ring_iterator<Iterator> operator--(ring_iterator<Iterator>& lhs, int)
{
  ring_iterator<Iterator> ret = lhs;
  --lhs;
  return ret;
}
template <typename Iterator, typename Distance>
ring_iterator<Iterator> operator-(ring_iterator<Iterator> const& lhs, Distance rhs)
{
  return lhs + (-rhs);
}
///--------------------------------------------
template <typename Iterator>
ring_iterator<Iterator> norm(ring_iterator<Iterator> const& it)
{
  ring_iterator<Iterator> ret = it;
  normalize(ret);
  return ret;
}
}
