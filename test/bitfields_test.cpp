#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <tuple>

using size_type = std::size_t;

template <size_type N>
using Number = std::integral_constant<size_type, N>;

namespace detail {

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
using storage = typename GetStorage<Size>::type;

template <typename...>
struct Sum;

template <typename Head, typename... Args>
struct Sum<Head, Args...> {
  using tail = typename Sum<Args...>::type;
  using type = Number<Head::value + tail::value>;
};

template <typename Head>
struct Sum<Head> {
  using type = Number<Head::value>;
};

template <size_type Index, typename... Args>
struct At {
  static_assert(Index < sizeof...(Args), "Index out of bounds");
  using type = std::remove_reference_t<
      decltype(std::get<Index>(std::declval<std::tuple<Args...>>()))>;
};

template <size_type...>
struct Subset;

template <size_type Head, size_type... Indexes>
struct Subset<Head, Indexes...> {
  template <typename... Args>
  using tail = typename Subset<Indexes...>::template type<Args...>;
  template <typename... Args>
  using type = Number<At<Head, Args...>::type::value + tail<Args...>::value>;
};

template <size_type Head>
struct Subset<Head> {
  template <typename... Args>
  using type = Number<At<Head, Args...>::type::value>;
};
}

template <typename... Args>
struct Sum : public detail::Sum<Args...>::type {};

template <size_type Index, typename... Args>
struct At : public detail::At<Index, Args...>::type {};

template <size_type... Indexes>
struct Subset {
  template <typename... Args>
  struct type : public detail::Subset<Indexes...>::template type<Args...> {};
};

template <typename... Sizes>
struct Bitfields {
  using length = Sum<Sizes...>;
  using fields = std::tuple<Sizes...>;
  detail::storage<length::value> storage;
};

#include <gtest/gtest.h>

struct bitfields_test : public ::testing::Test {};

TEST_F(bitfields_test, nothing) {
  static_assert(sizeof(Bitfields<Number<1>>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(Bitfields<Number<8>>) <= sizeof(std::uint8_t), "");
  static_assert(Sum<Number<1>, Number<2>, Number<3>>::value == 6, "");
  static_assert(At<2, Number<1>, Number<2>, Number<3>>::value == 3, "");
  static_assert(
      Subset<0, 2>::type<Number<1>, Number<2>, Number<3>, Number<4>>::value ==
          4,
      "");
}
