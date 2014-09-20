#ifndef ARCHIE_UTILS_OPERATORS_H_INCLUDED
#define ARCHIE_UTILS_OPERATORS_H_INCLUDED

namespace archie {
namespace utils {

  template <typename T>
  struct Operators {
    friend bool operator!=(T const& lhs, T const& rhs) {
      return (lhs < rhs) || (lhs > rhs);
    }
    friend bool operator==(T const& lhs, T const& rhs) { return !(lhs != rhs); }
    friend bool operator>(T const& lhs, T const& rhs) { return rhs < lhs; }
    friend bool operator>=(T const& lhs, T const& rhs) { return !(lhs < rhs); }
    friend bool operator<=(T const& lhs, T const& rhs) { return !(lhs > rhs); }
  };
}
}

#endif
