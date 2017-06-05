#include <archie/function_ref.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace
{
  using namespace archie;

  using arg_t = std::unique_ptr<int>;

  int foo(arg_t) { return 2; }
  struct bar {
    int operator()(arg_t) const { return 7; };
  } boo;
  unsigned foobar(arg_t) { return 7; }
  using fr_t = function_ref<int(arg_t)>;

  TEST(function_ref, construction)
  {
    {
      fr_t fr{&foo};
      EXPECT_EQ(fr(std::make_unique<int>(0)), foo(std::make_unique<int>(0)));
    }
    {
      fr_t fr{&foobar};
      EXPECT_EQ(fr(std::make_unique<int>(0)), static_cast<int>(foobar(std::make_unique<int>(0))));
    }
    {
      function_ref orig(&foobar);
      fr_t fr{orig};
      EXPECT_EQ(fr(std::make_unique<int>(0)), static_cast<int>(orig(std::make_unique<int>(0))));
    }
    {
      auto const l = [](arg_t) { return 3; };
      fr_t fr{l};
      EXPECT_EQ(fr(std::make_unique<int>(0)), l(std::make_unique<int>(0)));
    }
    {
      fr_t fr{boo};
      EXPECT_EQ(fr(std::make_unique<int>(0)), boo(std::make_unique<int>(0)));
    }
  }

  TEST(function_ref, assigment)
  {
    fr_t fr{&foo};
    {
      fr = &foo;
      EXPECT_EQ(fr(std::make_unique<int>(0)), foo(std::make_unique<int>(0)));
    }
    {
      auto const l = [](arg_t) { return 3; };
      fr = l;
      EXPECT_EQ(fr(std::make_unique<int>(0)), l(std::make_unique<int>(0)));
    }
    {
      fr = boo;
      EXPECT_EQ(fr(std::make_unique<int>(0)), boo(std::make_unique<int>(0)));
    }
  }
}
