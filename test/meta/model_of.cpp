#include <archie/traits/model_of.hpp>
#include <archie/meta/requires.hpp>
#include <archie/ignore.hpp>
#include <archie/models.hpp>
#include <vector>
#include <gtest/gtest.h>

namespace {
using archie::meta::requires;
using archie::traits::model_of;
using archie::models::Callable;
using archie::models::Iterable;

namespace fused = archie::fused;

struct foo {
  template <typename Tp>
  int func(Tp, requires<model_of<Callable(Tp)>> = fused::ignore) const
  {
    return 0;
  }
  template <typename Tp>
  int func(Tp, requires<model_of<Callable(Tp, int)>> = fused::ignore) const
  {
    return 1;
  }
};

struct goo {
  int operator()() { return 3; }
};

struct hoo {
  int operator()(int) { return 4; }
};

TEST(model_of, canUseModelOf)
{
  foo f;
  goo g;
  hoo h;

  EXPECT_EQ(0, f.func(g));
  EXPECT_EQ(1, f.func(h));
}

template <typename Tp>
struct is_iterable : model_of<Iterable(Tp)> {
};

struct no_iter {
};
struct iter {
  no_iter* begin();
  no_iter* end();
};

static_assert(!is_iterable<no_iter>::value, "");
static_assert(is_iterable<std::vector<no_iter>>::value, "");
static_assert(is_iterable<iter>::value, "");
}
