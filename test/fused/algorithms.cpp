#include <archie/fused/tuple.hpp>
#include <archie/fused/algorithm.hpp>
#include <memory>
#include <gtest/gtest.h>

namespace
{
  namespace fused = archie::fused;

  TEST(algorithms, canUseFusedCompose)
  {
    {
      auto x = fused::compose(fused::make_tuple, 1, 2u, '3');
      static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
      EXPECT_EQ(1, fused::get<0>(x));
      EXPECT_EQ(2u, fused::get<1>(x));
      EXPECT_EQ('3', fused::get<2>(x));
    }
    {
      auto a = 1;
      auto b = 2u;
      auto c = '3';

      auto const& x = fused::compose(fused::make_tuple(fused::front, fused::tie), a, b, c);
      EXPECT_EQ(&a, &x);
    }
    {
      auto a = 1;
      auto b = 2u;
      auto c = '3';

      auto opt = fused::make_tuple(fused::front, fused::tie);
      auto const& x = fused::compose(opt, a, b, c);
      EXPECT_EQ(&a, &x);
    }
  }

  TEST(algorithms, canComposeFusedMakeTuple)
  {
    auto x = fused::apply(fused::make_tuple, 1, 2u, '3');
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));
    EXPECT_EQ('3', fused::get<2>(x));
  }

  TEST(algorithms, canComposeFusedTie)
  {
    auto a = 1;
    auto b = 2u;
    auto c = '3';
    auto x = fused::apply(fused::tie, a, b, c);
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
    EXPECT_EQ(&a, &fused::get<0>(x));
    EXPECT_EQ(&b, &fused::get<1>(x));
    EXPECT_EQ(&c, &fused::get<2>(x));
  }

  TEST(algorithms, canComposeFusedFront)
  {
    {
      auto a = fused::apply(fused::front, fused::make_tuple(1, 2u, '3'));
      EXPECT_EQ(1, a);
      auto const& b = fused::apply(fused::front, a, 2u, '3');
      EXPECT_EQ(1, b);
      EXPECT_EQ(&a, &b);
    }
#if 0
  {
    auto x = fused::compose(fused::make_tuple(fused::front, fused::make_tuple), 1, 2u, '3');
EXPECT_EQ(1, x);
  }
#endif
  }

  TEST(algorithms, canComposeFusedBack)
  {
    auto a = fused::apply(fused::back, fused::make_tuple(1, 2u, '3'));
    EXPECT_EQ('3', a);
    auto const& b = fused::apply(fused::back, 1, 2u, a);
    EXPECT_EQ('3', b);
    EXPECT_EQ(&a, &b);
  }

  TEST(algorithms, canComposeFusedForEach)
  {
    {
      auto i = 0u;
      auto f = [&i](auto&&) { ++i; };
      fused::apply(fused::for_each, fused::make_tuple(f, 2u, '3'));
      fused::apply(fused::for_each, f, 2u, '3');
      EXPECT_EQ(4u, i);
    }
    {
      auto i = 0u;
      auto f = [&i](auto&&) { ++i; };
      auto opt = fused::make_tuple(fused::for_each, fused::make_tuple);
      fused::compose(opt, f, 1, 2u, '3', 4.0);
      EXPECT_EQ(4u, i);
    }
  }

  TEST(algorithms, canComposeFusedForEachOrder)
  {
    auto i = 0u;
    auto f = [&i](auto&& x) {
      i *= 10;
      i += x;
    };
    fused::for_each_forward(f, 1u, 2u, 3u);
    EXPECT_EQ(123u, i);
    i = 0u;
    fused::for_each_backward(f, 1u, 2u, 3u);
    EXPECT_EQ(321u, i);
  }

  TEST(algorithms, canComposeFusedTransform)
  {
    {
      auto f = [](auto&& x) { return ++x; };
      auto x = fused::apply(fused::transform, f, 1, 2u, '3');
      static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
      EXPECT_EQ(2, fused::get<0>(x));
      EXPECT_EQ(3u, fused::get<1>(x));
      EXPECT_EQ('4', fused::get<2>(x));
    }
    {
      auto f = [](auto&& x) { return std::make_unique<std::remove_reference_t<decltype(x)>>(x); };
      auto opt = fused::make_tuple(fused::transform, fused::make_tuple);
      auto x = fused::compose(opt, f, 1, 2u, '3', 4.0);
      static_assert(fused::tuple_size(fused::id<decltype(x)>) == 4u, "");
      EXPECT_NE(fused::get<0>(x), nullptr);
      EXPECT_EQ(1, *fused::get<0>(x));
    }
  }

  TEST(algorithms, canComposeFusedConcat)
  {
    auto x = fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), 4.0);
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 4u, "");
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));
    EXPECT_EQ('3', fused::get<2>(x));
    EXPECT_EQ(4.0, fused::get<3>(x));

    auto y =
        fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), fused::make_tuple(4.0, 5, 6u));
    static_assert(fused::tuple_size(fused::id<decltype(y)>) == 6u, "");
    EXPECT_EQ(1, fused::get<0>(y));
    EXPECT_EQ(2u, fused::get<1>(y));
    EXPECT_EQ('3', fused::get<2>(y));
    EXPECT_EQ(4.0, fused::get<3>(y));
    EXPECT_EQ(5, fused::get<4>(y));
    EXPECT_EQ(6u, fused::get<5>(y));
  }

  TEST(algorithms, canComposeFusedZip)
  {
    auto x =
        fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), fused::make_tuple(4.0, 5, 6u));
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 6u, "");
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));
    EXPECT_EQ('3', fused::get<2>(x));
    EXPECT_EQ(4.0, fused::get<3>(x));
    EXPECT_EQ(5, fused::get<4>(x));
    EXPECT_EQ(6u, fused::get<5>(x));
  }

  TEST(algorithms, canComposeFusedTail)
  {
    auto x = fused::tail(fused::make_tuple(1, 2u, '3'));
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
    EXPECT_EQ(2u, fused::get<0>(x));
    EXPECT_EQ('3', fused::get<1>(x));
  }

  TEST(algorithms, canComposeFusedFind)
  {
    auto x = fused::apply(fused::find<unsigned>, 1, 2u, '3', 4u);
    EXPECT_EQ(2u, x);
    auto y = fused::apply(fused::find<char>, 1, 2u, '3', 4u);
    EXPECT_EQ('3', y);
  }

  TEST(algorithms, canComposeFusedFindIf)
  {
    auto x = fused::apply(fused::find_if<std::is_unsigned>, 1, 2u, '3', 4u);
    EXPECT_EQ(2u, x);
    auto y = fused::apply(fused::find_if<std::is_signed>, 1, 2u, '3', 4u);
    EXPECT_EQ(1, y);
  }

  TEST(algorithms, canComposeFusedTake)
  {
    auto x = fused::apply(fused::take<2>, fused::make_tuple(1, 2u, '3'));
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));

    auto y = fused::apply(fused::take<3>, 4.0, '3', 2u, 1);
    static_assert(fused::tuple_size(fused::id<decltype(y)>) == 3u, "");
    EXPECT_EQ(4.0, fused::get<0>(y));
    EXPECT_EQ('3', fused::get<1>(y));
    EXPECT_EQ(2u, fused::get<2>(y));
  }

  TEST(algorithms, canComposeFusedNth)
  {
    auto x = fused::apply(fused::nth<0>, 1, 2u, '3');
    auto y = fused::apply(fused::nth<1>, 1, 2u, '3');
    auto z = fused::apply(fused::nth<2>, 1, 2u, '3');
    EXPECT_EQ(1, x);
    EXPECT_EQ(2u, y);
    EXPECT_EQ('3', z);
  }

  TEST(algorithms, canComposeFusedConstruct)
  {
    using tuple_type = fused::tuple<int, unsigned, char>;
    constexpr auto& ctor = fused::id<tuple_type>;
    auto x = fused::apply(ctor, 1, 2u, '3');
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));
    EXPECT_EQ('3', fused::get<2>(x));
  }

  TEST(algorithms, canComposeIndexOf)
  {
    {
      auto x = fused::apply(fused::type_index<int>, 1, 2u, '3');
      auto y = fused::apply(fused::type_index<unsigned>, 1, 2u, '3');
      auto z = fused::apply(fused::type_index<char>, 1, 2u, '3');
      EXPECT_EQ(0u, static_cast<std::size_t>(x));
      EXPECT_EQ(1u, static_cast<std::size_t>(y));
      EXPECT_EQ(2u, static_cast<std::size_t>(z));
    }
    {
      auto x = fused::apply(fused::type_index<int>, 1);
      EXPECT_EQ(0u, static_cast<std::size_t>(x));
    }
    {
      auto x = fused::apply(fused::index_of<int>, fused::type_list<int, unsigned, char>);
      auto y = fused::apply(fused::index_of<unsigned>, fused::type_list<int, unsigned, char>);
      auto z = fused::apply(fused::index_of<char>, fused::type_list<int, unsigned, char>);
      EXPECT_EQ(0u, static_cast<std::size_t>(x));
      EXPECT_EQ(1u, static_cast<std::size_t>(y));
      EXPECT_EQ(2u, static_cast<std::size_t>(z));
    }
  }
}
