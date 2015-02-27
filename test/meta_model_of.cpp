#include <archie/utils/meta/model_of.h>
#include <archie/utils/meta/requires.h>
#include <archie/utils/fused/ignore.h>
#include <archie/utils/models.h>
#include <archie/utils/test.h>

using archie::utils::meta::requires;
using archie::utils::meta::requires_none;
using archie::utils::meta::model_of;
using archie::utils::models::Callable;

namespace meta = archie::utils::meta;
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

template <class F1, class F2>
struct basic_conditional {
  template <typename... Ts>
  auto operator()(Ts&&... xs) -> decltype(F1()(std::forward<Ts>(xs)...)) {
    return F1()(std::forward<Ts>(xs)...);
  }
  template <typename... Ts>
  auto operator()(Ts&&... xs) -> decltype(F2()(std::forward<Ts>(xs)...)) {
    return F2()(std::forward<Ts>(xs)...);
  }
};

template <class F, class... Fs>
struct conditional : basic_conditional<F, conditional<Fs...>> {};

template <class F>
struct conditional<F> : F {};

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

void canUseConditional() {
  conditional<goo, hoo> cond;
  auto x = cond();
  auto y = cond(1);
  ASSERT_EQ(3, x);
  ASSERT_EQ(4, y);
}

int main() {
  canUseModelOf();
  return 0;
}
