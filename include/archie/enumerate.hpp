#pragma once
#include <utility>
#include <type_traits>
#include <iterator>

namespace archie
{
  namespace detail
  {
    template <typename>
    class enumerator;
  }

  template <typename Range>
  detail::enumerator<Range> enumerate(Range&);

  namespace detail
  {
    template <typename Range>
    class enumerator {
      Range& range_;

      explicit enumerator(Range& r) : range_(r) {}
      class iterator {
        friend class enumerator;
        using size_type = typename Range::size_type;

        using range_iterator = std::conditional_t<std::is_const<Range>::value,
                                                  typename Range::const_iterator,
                                                  typename Range::iterator>;
        using range_reference = decltype(*std::declval<range_iterator>());

        range_iterator iter_;
        size_type offset_;

        using item_t = std::pair<size_type, range_reference>;

        class reference : item_t {
          friend class iterator;
          using item_t::item_t;

        public:
          using item_t::first;
          using item_t::second;

          template <typename R,
                    std::enable_if_t<std::is_assignable<range_reference, R>::value, int> = 0>
          reference& operator=(R& r)
          {
            this->second = r;
            return *this;
          }
        };

        explicit iterator(range_iterator i) : iter_(i), offset_(0) {}
      public:
        bool operator==(iterator const& rhs) const { return iter_ == rhs.iter_; }
        bool operator!=(iterator const& rhs) const { return !(*this == rhs); }
        iterator& operator++()
        {
          ++iter_;
          ++offset_;
          return *this;
        }

        reference operator*() const { return reference{offset_, *iter_}; }
      };

    public:
      iterator begin() const { return iterator(std::begin(range_)); }
      iterator end() const { return iterator(std::end(range_)); }
      template <typename R>
      friend enumerator<R> archie::enumerate(R&);
    };
  }

  template <typename Range>
  detail::enumerator<Range> enumerate(Range& r)
  {
    return detail::enumerator<Range>(r);
  }
}
