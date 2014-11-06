#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <tuple>

namespace archie {
namespace utils {

  using size_type = std::uint8_t;

  template <size_type N>
  using Number = std::integral_constant<size_type, N>;

  using ByteSize = std::integral_constant<size_type, 8>;

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
                                     PackImpl<I + 1, O + S, T...> {};

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

  template <size_type... S>
  struct Pack : detail::PackImpl<0, 0, S...> {
    constexpr Pack() noexcept = default;
    constexpr size_type size() noexcept { return Sum<S...>::value; }
    constexpr size_type length() noexcept { return sizeof...(S); }
    using data_type = Storage<Sum<S...>::value>;

    template <size_type I>
    struct Field {
      size_type offset() const noexcept { return offset_of<I>(Pack{}); }
      size_type size() const noexcept { return size_of<I>(Pack{}); }
      size_type index() const noexcept { return I; }
      friend Pack;

    private:
      explicit Field(Pack::data_type& d) noexcept : data(d) {}
      data_type& data;
    };

    template <size_type I>
    Field<I> make_field() noexcept {
      return Field<I>(data);
    }

  private:
    data_type data = 0;
  };
}
}

#include <gtest/gtest.h>
namespace {

namespace au = archie::utils;

struct bitfields_test : public ::testing::Test {};

TEST_F(bitfields_test, storage) {
  static_assert(sizeof(au::Storage<0>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(au::Storage<8>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(au::Storage<9>) <= sizeof(std::uint16_t), "");
  static_assert(sizeof(au::Storage<16>) <= sizeof(std::uint16_t), "");
  static_assert(sizeof(au::Storage<17>) <= sizeof(std::uint32_t), "");
  static_assert(sizeof(au::Storage<32>) <= sizeof(std::uint32_t), "");
  static_assert(sizeof(au::Storage<33>) <= sizeof(std::uint64_t), "");
  static_assert(sizeof(au::Storage<64>) <= sizeof(std::uint64_t), "");
}

TEST_F(bitfields_test, pack) {
  static_assert(sizeof(au::Pack<0>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(au::Pack<3, 5>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(au::Pack<4, 5>) <= sizeof(std::uint16_t), "");
  static_assert(sizeof(au::Pack<3, 5, 8>) <= sizeof(std::uint16_t), "");
  static_assert(sizeof(au::Pack<10, 5, 2>) <= sizeof(std::uint32_t), "");
  static_assert(sizeof(au::Pack<15, 15, 2>) <= sizeof(std::uint32_t), "");
  static_assert(sizeof(au::Pack<10, 10, 13>) <= sizeof(std::uint64_t), "");
  static_assert(sizeof(au::Pack<30, 20, 10, 4>) <= sizeof(std::uint64_t), "");
}

TEST_F(bitfields_test, pack_element_offset) {
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(au::offset_of<0>(pack) == 0, "");
  static_assert(au::offset_of<1>(pack) == 1, "");
  static_assert(au::offset_of<2>(pack) == 3, "");
}

TEST_F(bitfields_test, pack_element_size) {
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(au::size_of<0>(pack) == 1, "");
  static_assert(au::size_of<1>(pack) == 2, "");
  static_assert(au::size_of<2>(pack) == 3, "");
}

TEST_F(bitfields_test, pack_size) {
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(pack.size() == 6, "");
}

TEST_F(bitfields_test, pack_length) {
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(pack.length() == 3, "");
}

TEST_F(bitfields_test, pack_make_field) {
  au::Pack<1, 2, 3> pack;
  auto field = pack.make_field<0>();
  EXPECT_EQ(0u, field.index());
}
}
