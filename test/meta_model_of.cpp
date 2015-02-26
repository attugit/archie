#include <archie/utils/meta/model_of.h>
#include <archie/utils/meta/requires.h>
#include <archie/utils/fused/ignore.h>
#include <archie/utils/test.h>

using archie::utils::meta::requires;
using archie::utils::meta::model_of;

namespace meta = archie::utils::meta;
namespace fused = archie::utils::fused;

struct Callable {
  template <class F, class... Ts>
  auto requires(F&& f, Ts&&... xs) -> decltype(f(std::forward<Ts>(xs)...));
};

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
  void operator()() {}
};

struct hoo {
  void operator()(int) {}
};

void canUseModelOf() {
  foo f;
  goo g;
  hoo h;

  ASSERT_EQ(0, f.func(g));
  ASSERT_EQ(1, f.func(h));
}

int main() {
  canUseModelOf();
  return 0;
}
