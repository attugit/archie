#include <archie/pure_function.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace {
using namespace archie;

using arg_t = std::unique_ptr<int>;

int foo(arg_t)
{
  return 2;
}

struct bar {
  int operator()(arg_t) const { return 7; };
} boo;

using pf_t = pure_function<int(arg_t)>;

TEST(pure_function, construction)
{
  {
    pf_t pf{foo};
    EXPECT_TRUE(pf);
    EXPECT_EQ(pf(std::make_unique<int>(0)), foo(std::make_unique<int>(0)));
  }
  {
    auto const l = [](arg_t) { return 3; };
    pf_t pf{l};
    EXPECT_TRUE(pf);
    EXPECT_EQ(pf(std::make_unique<int>(0)), l(std::make_unique<int>(0)));
  }
  {
    pf_t pf{boo};
    EXPECT_TRUE(pf);
    EXPECT_EQ(pf(std::make_unique<int>(0)), boo(std::make_unique<int>(0)));
  }
}

TEST(pure_function, assigment)
{
  pf_t pf;
  {
    EXPECT_FALSE(pf);
  }
  {
    pf = foo;
    EXPECT_TRUE(pf);
    EXPECT_EQ(pf(std::make_unique<int>(0)), foo(std::make_unique<int>(0)));
  }
  {
    auto const l = [](arg_t) { return 3; };
    pf = l;
    EXPECT_TRUE(pf);
    EXPECT_EQ(pf(std::make_unique<int>(0)), l(std::make_unique<int>(0)));
  }
  {
    pf = boo;
    EXPECT_TRUE(pf);
    EXPECT_EQ(pf(std::make_unique<int>(0)), boo(std::make_unique<int>(0)));
  }
  {
    pf = nullptr;
    EXPECT_FALSE(pf);
  }
}

TEST(pure_function, conversionToFunctionPointer)
{
  pf_t pf;
  pf_t::type ptr = pf;
  EXPECT_FALSE(ptr);
  pf = boo;
  ptr = pf;
  EXPECT_TRUE(ptr);
  EXPECT_EQ(ptr(std::make_unique<int>(0)), boo(std::make_unique<int>(0)));
}

TEST(pure_function, returningVoid)
{
  pure_function<void(int)> pf{[](int) {}};
  EXPECT_TRUE(pf);
  pf(42);
}
}
