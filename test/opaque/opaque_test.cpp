#include <archie/opaque.hpp>
#include <gtest/gtest.h>
#include <functional>

namespace
{
  using namespace archie;
  struct ATag {
  };
  struct BTag {
  };
  struct CTag {
  };

  using BOpaque = opaque<BTag, double, feature::equivalent<tag<ATag>>, feature::equivalent<self>>;

  using AOpaque = opaque<ATag,
                         double,
                         feature::extractable,
                         feature::equivalent<feature::self>,
                         feature::equivalent<double>>;

  using COpaque = opaque<CTag,
                         double,
                         feature::extractable,
                         feature::assignable<double>,
                         feature::ordered<feature::self>,
                         feature::ordered<double>,
                         feature::ordered<tag<ATag>>>;

  TEST(opaque, constructors)
  {
    {
      AOpaque aopq;
      EXPECT_EQ(sizeof(aopq), sizeof(double));
    }
    {
      AOpaque aopq(4.0);
      EXPECT_EQ(sizeof(aopq), sizeof(double));
    }
    {
      AOpaque const aopq(5.0);
      AOpaque other = aopq;
      EXPECT_EQ(sizeof(other), sizeof(double));
    }
    {
      AOpaque aopq(5.0);
      AOpaque other = std::move(aopq);
      EXPECT_EQ(sizeof(other), sizeof(double));
    }
  }

  TEST(opaque, extractable)
  {
    {
      AOpaque const aopq(4.0);
      EXPECT_EQ(extract(aopq), 4.0);
    }
    {
      AOpaque aopq(4.0);
      extract(aopq) = 3.0;
      EXPECT_EQ(extract(aopq), 3.0);
    }
    {
      double const x = 10.0;
      auto const& y = extract(x);
      EXPECT_EQ(&x, &y);
      double z = 3.0;
      auto& v = extract(z);
      v = 5.0;
      EXPECT_EQ(&z, &v);
    }
    {
      BOpaque const x(1.0);
      auto const& y = extract(x);
      EXPECT_EQ(&x, &y);
      BOpaque z(1.0);
      BOpaque& v = extract(z);
      EXPECT_EQ(&v, &z);
    }
  }

  TEST(opaque, operators)
  {
    AOpaque const aopq(3.0);
    BOpaque const bopq(4.0);
    {
      EXPECT_EQ(aopq, 3.0);
      EXPECT_EQ(aopq, aopq);
      EXPECT_NE(aopq, 4.0);
      EXPECT_NE(bopq, aopq);
    }
    {
      AOpaque const a{5.0};
      COpaque const x{2.0};
      COpaque const y{3.0};

      EXPECT_LT(x, y);
      EXPECT_LE(x, y);
      EXPECT_LE(x, x);
      EXPECT_GT(y, x);
      EXPECT_GE(y, x);
      EXPECT_GE(y, y);

      EXPECT_GE(y, x);
      EXPECT_GE(x, x);
      EXPECT_GT(y, x);
      EXPECT_LE(x, y);
      EXPECT_LE(x, x);
      EXPECT_LT(x, y);

      EXPECT_LT(x, 3.0);
      EXPECT_LE(x, 3.0);
      EXPECT_LE(x, 2.0);
      EXPECT_GT(x, 1.0);
      EXPECT_GE(x, 1.0);
      EXPECT_GE(x, 2.0);

      EXPECT_LT(1.0, x);
      EXPECT_LE(1.0, x);
      EXPECT_LE(2.0, x);
      EXPECT_GT(3.0, x);
      EXPECT_GE(3.0, x);
      EXPECT_GE(2.0, x);

      EXPECT_GE(x, 1.0);
      EXPECT_GE(x, 2.0);
      EXPECT_GT(x, 1.0);
      EXPECT_LE(x, 3.0);
      EXPECT_LE(x, 2.0);
      EXPECT_LT(x, 3.0);

      EXPECT_GE(3.0, x);
      EXPECT_GE(2.0, x);
      EXPECT_GT(3.0, x);
      EXPECT_LE(1.0, x);
      EXPECT_LE(2.0, x);
      EXPECT_LT(1.0, x);

      EXPECT_LT(x, a);
      EXPECT_LE(x, a);
      EXPECT_LE(x, a);
      EXPECT_LT(x, a);
      EXPECT_GT(a, x);
      EXPECT_GE(a, x);
      EXPECT_GE(a, x);
      EXPECT_GT(a, x);
    }
    {
      AOpaque other(3.0);
      EXPECT_EQ(other, 3.0);
      extract(other) = 4.0;
      EXPECT_EQ(other, 4.0);
      other = AOpaque{5.0};
      EXPECT_EQ(other, 5.0);
      COpaque copq(2.0);
      copq = 6.0;
      EXPECT_EQ(extract(copq), 6.0);
    }
  }
}
