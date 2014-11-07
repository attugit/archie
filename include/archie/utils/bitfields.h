#ifndef ARCHIE_UTILS_BITFIELDS_H_INCLUDED
#define ARCHIE_UTILS_BITFIELDS_H_INCLUDED

#include <utility>
#include <cstddef>
#include <cstdint>

namespace archie {
namespace utils {

  using size_type = std::uint8_t;

  template <size_type N>
  using Number = std::integral_constant<size_type, N>;

  using ByteSize = Number<8u>;

  template <size_type Size>
  struct GetStorage {
    static_assert(Size <= ByteSize::value * sizeof(std::uint64_t),
                  "Max possible size exceeded");
    using type = typename std::conditional<
        (Size > ByteSize::value * sizeof(std::uint8_t)),
        typename std::conditional<
            (Size > ByteSize::value * sizeof(std::uint16_t)),
            typename std::conditional<
                (Size > ByteSize::value * sizeof(std::uint32_t)), std::uint64_t,
                std::uint32_t>::type,
            std::uint16_t>::type,
        std::uint8_t>::type;
  };

  template <size_type Size>
  using Storage = typename GetStorage<Size>::type;

  template <size_type I, size_type O, size_type S>
  struct PackElement {
    using Index = Number<I>;
    using Offset = Number<O>;
    using Size = Number<S>;
  };

  template <size_type I, size_type O, size_type S>
  constexpr size_type offset_of(PackElement<I, O, S>) noexcept {
    return O;
  }

  template <size_type I, size_type O, size_type S>
  constexpr size_type size_of(PackElement<I, O, S>) noexcept {
    return S;
  }

  namespace detail {
    template <size_type...>
    struct PackImpl;

    template <size_type I, size_type O, size_type S>
    struct PackImpl<I, O, S> : PackElement<I, O, S> {};

    template <size_type I, size_type O, size_type S, size_type... T>
    struct PackImpl<I, O, S, T...> : PackElement<I, O, S>,
                                     PackImpl<I + 1u, O + S, T...> {};

    template <size_type...>
    struct Sum;

    template <size_type N>
    struct Sum<N> {
      using type = Number<N>;
    };

    template <size_type N, size_type... T>
    struct Sum<N, T...> {
      using type = Number<N + Sum<T...>::type::value>;
    };
  }

  template <size_type... N>
  using Sum = typename detail::Sum<N...>::type;

  template <typename Integral>
  struct IntegralWrapper {
    using value_type = Integral;
    constexpr IntegralWrapper() noexcept = default;
    constexpr explicit IntegralWrapper(value_type v) noexcept : value(v) {}
    constexpr operator value_type() const noexcept { return value; }

  private:
    value_type value = 0;
  };

  struct IndexType : IntegralWrapper<size_type> {
    using IntegralWrapper::IntegralWrapper;
  };

  template <size_type... S>
  struct Pack : detail::PackImpl<0u, 0u, S...> {
    constexpr Pack() noexcept = default;
    constexpr size_type size() noexcept { return Sum<S...>::value; }
    constexpr size_type length() noexcept { return sizeof...(S); }
    using data_type = Storage<Sum<S...>::value>;
    void set(data_type val) noexcept { data = val; }
    data_type get() const noexcept { return data; }

    template <size_type I>
    constexpr static data_type mask_of() noexcept {
      return (((1u << size_of<I>(Pack{})) - 1u) << offset_of<I>(Pack{}));
    }

    template <size_type I>
    struct Field {
      size_type offset() const noexcept { return offset_of<I>(Pack{}); }
      size_type size() const noexcept { return size_of<I>(Pack{}); }
      size_type index() const noexcept { return I; }
      data_type mask() const noexcept { return Pack::mask_of<I>(); }
      using field_type = Storage<size_of<I>(Pack{})>;
      field_type value() const noexcept { return (data & mask()) >> offset(); }

      bool test(IndexType at) const noexcept {
        return (value() & (1u << at)) != 0u;
      }

      Field& set(IndexType at) noexcept {
        data = data | ((1u << (at + offset())) & mask());
        return *this;
      }
      Field& reset(IndexType at) noexcept {
        data = data & (~((1u << (at + offset())) & mask()));
        return *this;
      }
      Field& set(IndexType at, bool value) noexcept {
        return value ? set(at) : reset(at);
      }
      bool all() const noexcept { return (data & mask()) == mask(); }
      bool any() const noexcept { return (data & mask()) != 0u; }
      bool none() const noexcept { return !any(); }

      template <size_type Idx>
      bool operator==(Field<Idx> const& rhs) const noexcept {
        static_assert(Size::value == Field<Idx>::Size::value, "");
        return value() == rhs.value();
      }
      template <size_type Idx>
      bool operator!=(Field<Idx> const& rhs) const noexcept {
        return !(*this == rhs);
      }
      template <size_type Idx>
      bool operator<(Field<Idx> const& rhs) const noexcept {
        static_assert(Size::value == Field<Idx>::Size::value, "");
        return value() < rhs.value();
      }
      template <size_type Idx>
      bool operator>=(Field<Idx> const& rhs) const noexcept {
        return !(*this < rhs);
      }
      template <size_type Idx>
      bool operator>(Field<Idx> const& rhs) const noexcept {
        return rhs < *this;
      }
      template <size_type Idx>
      bool operator<=(Field<Idx> const& rhs) const noexcept {
        return !(rhs < *this);
      }

      friend Pack;

    private:
      using Size = Number<size_of<I>(Pack{})>;
      explicit Field(Pack::data_type& d) noexcept : data(d) {}
      data_type& data;
    };

    template <size_type I>
    Field<I> make_field() noexcept {
      return Field<I>(data);
    }

    template <typename FieldType>
    Pack& set(IndexType at) noexcept {
      FieldType(data).set(at);
      return *this;
    }
    template <typename FieldType>
    Pack& reset(IndexType at) noexcept {
      FieldType(data).reset(at);
      return *this;
    }
    template <typename FieldType>
    Pack& set(IndexType at, bool value) noexcept {
      FieldType(data).set(at, value);
      return *this;
    }

  private:
    data_type data = 0u;
  };
}
}

#endif
