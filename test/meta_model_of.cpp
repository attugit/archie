#include <archie/utils/traits/model_of.h>
#include <archie/utils/meta/requires.h>
#include <archie/utils/fused/ignore.h>
#include <archie/utils/models.h>
#include <archie/utils/test.h>

using archie::utils::meta::requires;
using archie::utils::traits::model_of;
using archie::utils::models::Callable;
using archie::utils::models::Iterable;

namespace fused = archie::utils::fused;

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

void canUseModelOf() {
  foo f;
  goo g;
  hoo h;

  ASSERT_EQ(0, f.func(g));
  ASSERT_EQ(1, f.func(h));
}

template <typename Tp>
struct is_iterable : model_of<Iterable(Tp)> {};

struct no_iter {};
struct iter {
  no_iter* begin();
  no_iter* end();
};

#include <vector>
static_assert(!is_iterable<no_iter>::value, "");
static_assert(is_iterable<std::vector<no_iter>>::value, "");
static_assert(is_iterable<iter>::value, "");

int main() {
  canUseModelOf();
  return 0;
}
