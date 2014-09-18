#include <utility>
#include <archie/utils/requires.h>
#include <type_traits>

namespace archie {
namespace utils {

template <typename Head, typename... Tail>
struct TypeSet : TypeSet<Head>, TypeSet<Tail...> {
  TypeSet(Head head, Tail... tail)
      : TypeSet<Head>(std::forward<Head>(head)),
        TypeSet<Tail...>(std::forward<Tail>(tail)...) {}

  template <typename T, Requires<std::is_same<T, Head>>...>
  const T& get() const {
    return TypeSet<Head>::template get<T>();
  }
  template <typename T, Requires<std::is_same<T, Head>>...>
  T& get() {
    return TypeSet<Head>::template get<T>();
  }
  template <typename T, Requires<Not<std::is_same<T, Head>>>...>
  const T& get() const {
    return TypeSet<Tail...>::template get<T>();
  }
  template <typename T, Requires<Not<std::is_same<T, Head>>>...>
  T& get() {
    return TypeSet<Tail...>::template get<T>();
  }
};

template <typename Head>
struct TypeSet<Head> {
  template <typename U>
  explicit TypeSet(U u)
      : value(std::forward<U>(u)) {}

  template <typename T>
  const T& get() const {
    static_assert(std::is_same<T, Head>::value, "Type not found in TypeSet");
    return value;
  }
  template <typename T>
  T& get() {
    static_assert(std::is_same<T, Head>::value, "Type not found in TypeSet");
    return value;
  }

private:
  Head value;
};

template <typename T, typename... Types>
const T& get(const TypeSet<Types...>& ts) {
  return ts.get<T>();
}

template <typename T, typename... Types>
T& get(TypeSet<Types...>& ts) {
  return ts.get<T>();
}
}
}

#include <gtest/gtest.h>
namespace au = archie::utils;
struct typeset_test : ::testing::Test {};

TEST_F(typeset_test, nothing) {
  au::TypeSet<unsigned> unary(1);
  EXPECT_EQ(1, unary.get<unsigned>());

  au::TypeSet<unsigned, float> binary(2, 3.0);
  EXPECT_EQ(2, binary.get<unsigned>());
  EXPECT_EQ(3.0, binary.get<float>());

  binary.get<unsigned>() = 4;
  binary.get<float>() = 5.0;

  EXPECT_EQ(4, binary.get<unsigned>());
  EXPECT_EQ(5.0, binary.get<float>());

  unsigned u = 7;
  float f = 8.0;

  au::TypeSet<float, unsigned> other(f, u);
  EXPECT_EQ(7, au::get<unsigned>(other));
  EXPECT_EQ(8.0, au::get<float>(other));
}
