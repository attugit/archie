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
  constexpr size_type offset_of(PackElement<I, O, S>) {
    return O;
  }

  namespace detail {
    template <size_type...>
    struct PackImpl;

    template <size_type I, size_type O, size_type S>
    struct PackImpl<I, O, S> : PackElement<I, O, S> {};

    template <size_type I, size_type O, size_type S, size_type... T>
    struct PackImpl<I, O, S, T...> : PackElement<I, O, S>,
                                     PackImpl<I + 1, O + S, T...> {};
  }

  template <size_type... S>
  struct Pack : detail::PackImpl<0, 0, S...> {
    constexpr Pack() = default;
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

TEST_F(bitfields_test, pack_element_offset) {
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(au::offset_of<0>(pack) == 0, "");
  static_assert(au::offset_of<1>(pack) == 1, "");
  static_assert(au::offset_of<2>(pack) == 3, "");
}
}
