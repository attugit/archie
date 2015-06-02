#pragma once

namespace archie {
namespace utils {
  namespace containers {
    template <typename Tp>
    using SizeType = typename Tp::size_type;

    template <typename Tp>
    using DifferenceType = typename Tp::difference_type;

    template <typename Tp>
    using Iterator = typename Tp::iterator;

    template <typename Tp>
    using ConstIterator = typename Tp::const_iterator;

    template <typename Tp>
    using ValueType = typename Tp::value_type;

    template <typename Tp>
    using ConstIterator = typename Tp::const_iterator;

    template <typename Tp>
    using Pointer = typename Tp::pointer;

    template <typename Tp>
    using ConstPointer = typename Tp::const_pointer;

    template <typename Tp>
    using Reference = typename Tp::reference;

    template <typename Tp>
    using ConstReference = typename Tp::const_reference;

    template <typename Tp>
    using RangeType = typename Tp::range_type;

    template <typename Tp>
    using Allocator = typename Tp::allocator_type;
  }
}
}
