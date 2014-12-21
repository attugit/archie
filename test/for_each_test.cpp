#include <archie/utils/fused/for_each.h>
#include <string>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

struct func {
  int cnt = 0;
  template <typename Tp>
  void operator()(Tp&&) noexcept {
    ++cnt;
  }
};

void canCallForEachWithFunctionObject() {
  func f;
  ASSERT_EQ(0, f.cnt);
  fused::for_each(f);
  EXPECT_EQ(0, f.cnt);
  auto& ret = fused::for_each(f, '7', 'c', std::string{});
  EXPECT_EQ(3, f.cnt);
  EXPECT_EQ(3, ret.cnt);
  EXPECT_EQ(&f, &ret);
}

void canCallForEachWithLambda() {
  int idx = 0;
  auto f = [&idx](auto&&) { ++idx; };
  ASSERT_EQ(0, idx);
  fused::for_each(f, 7, 'c', 2.0);
  EXPECT_EQ(3, idx);
}

int main() {
  canCallForEachWithFunctionObject();
  canCallForEachWithLambda();
  return 0;
}
