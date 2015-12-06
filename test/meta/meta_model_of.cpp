#include <archie/traits/model_of.h>
#include <archie/meta/requires.h>
#include <archie/fused/ignore.h>
#include <archie/models.h>
#include <vector>
#include <catch.hpp>

namespace {
using archie::meta::requires;
using archie::traits::model_of;
using archie::models::Callable;
using archie::models::Iterable;

namespace fused = archie::fused;

struct foo {
  template <typename Tp>
  int func(Tp, requires<model_of<Callable(Tp)>> = fused::ignore) const {
    return 0;
  }
  template <typename Tp>
  int func(Tp, requires<model_of<Callable(Tp, int)>> = fused::ignore) const {
    return 1;
  }
};

struct goo {
  int operator()() { return 3; }
};

struct hoo {
  int operator()(int) { return 4; }
};

TEST_CASE("canUseModelOf") {
  foo f;
  goo g;
  hoo h;

  REQUIRE(0 == f.func(g));
  REQUIRE(1 == f.func(h));
}

template <typename Tp>
struct is_iterable : model_of<Iterable(Tp)> {};

struct no_iter {};
struct iter {
  no_iter* begin();
  no_iter* end();
};

static_assert(!is_iterable<no_iter>::value, "");
static_assert(is_iterable<std::vector<no_iter>>::value, "");
static_assert(is_iterable<iter>::value, "");
}
