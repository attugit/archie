#include <archie/fused/select_overload.hpp>
#include <archie/number.hpp>
#include <utility>
#include <type_traits>
#include <gtest/gtest.h>

namespace {
namespace fused = archie::fused;
namespace meta = archie::meta;

template <unsigned N, typename = typename std::enable_if<(N <= 8)>::type>
constexpr decltype(auto) foo(fused::choice<1>)
{
  return 1u;
}

template <unsigned N, typename = typename std::enable_if<(N <= 16)>::type>
constexpr decltype(auto) foo(fused::choice<2>)
{
  return 2u;
}

template <unsigned N>
constexpr decltype(auto) foo(fused::otherwise)
{
  return 11u;
}

TEST(select_overload, canSelectOverload)
{
  auto x = foo<8>(fused::select_overload);
  auto y = foo<16>(fused::select_overload);
  auto z = foo<17>(fused::select_overload);

  EXPECT_EQ(1u, x);
  EXPECT_EQ(2u, y);
  EXPECT_EQ(11u, z);
}

struct overloaded {
  template <typename Tp>
  constexpr decltype(auto) operator()(Tp&& t) const
  {
    return impl<Tp>(fused::select_overload, std::forward<Tp>(t));
  }

private:
  template <typename Tp, typename = typename std::enable_if<std::is_signed<Tp>::value>::type>
  constexpr decltype(auto) impl(fused::choice<1>, Tp&&) const
  {
    return meta::number<sizeof(Tp)>{};
  }
  template <typename Tp, typename = typename std::enable_if<std::is_fundamental<Tp>::value>::type>
  constexpr decltype(auto) impl(fused::choice<2>, Tp&& t) const
  {
    return std::forward<Tp>(t);
  }
  template <typename Tp>
  constexpr decltype(auto) impl(fused::otherwise, Tp&&) const
  {
    return 0u;
  }
};

TEST(select_overload, canSelectOverloadMember)
{
  constexpr overloaded over{};
  EXPECT_EQ(4u, over(1));
  EXPECT_EQ(7u, over(7u));
  EXPECT_EQ(0u, over(meta::number<7u>{}));
}
}
