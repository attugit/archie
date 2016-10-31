#pragma once
#include <algorithm>
#include <iterator>

namespace archie
{
  namespace detail
  {
    template <typename, typename>
    class filter_impl;
  }
  template <typename Range, typename Pred>
  auto filter(Range& r, Pred) -> detail::filter_impl<decltype(std::begin(r)), Pred>;

  namespace detail
  {
    template <typename ForwardIt, typename Pred>
    class filter_impl {
      class filter_iterator {
        friend class filter_impl;
        ForwardIt first_;
        ForwardIt last_;
        Pred pred_;

        explicit filter_iterator(ForwardIt first, ForwardIt last, Pred pred)
            : first_(first), last_(last), pred_(pred)
        {
        }

      public:
        auto& operator*() const { return *first_; }
        filter_iterator& operator++()
        {
          first_ = std::find_if(++first_, last_, pred_);
          return *this;
        }
        bool operator==(filter_iterator const& rhs) const { return first_ == rhs.first_; }
        bool operator!=(filter_iterator const& rhs) const { return !(*this == rhs); }
      };

      ForwardIt first_;
      ForwardIt last_;
      Pred pred_;

      template <typename Range, typename P>
      friend auto archie::filter(Range& r, P) -> filter_impl<decltype(std::begin(r)), P>;

      explicit filter_impl(ForwardIt first, ForwardIt last, Pred pred)
          : first_(std::find_if(first, last, pred)), last_(last), pred_(pred)
      {
      }

    public:
      filter_iterator begin() const { return filter_iterator(first_, last_, pred_); }
      filter_iterator end() const { return filter_iterator(last_, last_, pred_); }
    };
  }

  template <typename Range, typename Pred>
  auto filter(Range& r, Pred pred) -> detail::filter_impl<decltype(std::begin(r)), Pred>
  {
    return detail::filter_impl<decltype(std::begin(r)), Pred>(std::begin(r), std::end(r), pred);
  }
}
