#include <archie/resource.hpp>
#include <archie/alias.hpp>
#include <gtest/gtest.h>
#include <functional>
namespace
{
  using namespace archie;

  TEST(resource, canUseResource)
  {
    auto const del = [](auto& a) { ++unwrap(a); };
    {
      int i = 0;
      {
        auto res = make_resource(alias(i), del);
        EXPECT_EQ(i, 0);
      }
      EXPECT_EQ(i, 1);
    }

    {
      int i = 0;
      {
        auto res = make_resource(alias(i), del);
        EXPECT_EQ(i, 0);
        res.release();
      }
      EXPECT_EQ(i, 0);
    }

    {
      int i = 0;
      {
        auto res = make_resource(alias(i), del);
        EXPECT_EQ(i, 0);
        res = null_resource;
      }
      EXPECT_EQ(i, 0);
    }

    {
      int i = 0;
      {
        auto res = resource<int, std::function<void(int&)>>(in_place, 4);
        res.engage([&i](auto& x) { i += x; });
        EXPECT_EQ(i, 0);
      }
      EXPECT_EQ(i, 4);
    }

    {
      auto const nothing = [](auto) {};
      auto res = make_resource(1, nothing);
      EXPECT_EQ(*res, 1);
      auto const cres = make_resource(2, nothing);
      EXPECT_EQ(*cres, 2);

      struct point {
        int x = 0;
      };

      auto rpoint = make_resource(point{}, nothing);
      auto const crpoint = make_resource(point{}, nothing);
      EXPECT_EQ(rpoint->x, 0);
      EXPECT_EQ(crpoint->x, 0);
    }
  }
}
