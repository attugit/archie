#include <archie/utils/fused/strong_typedef.h>
#include <archie/utils/traits.h>
#include <type_traits>

namespace fused = archie::utils::fused;
namespace traits = archie::utils::traits;

struct int_t : fused::strong_typedef<int, fused::policy::eq<int>> {
  using self_t::self_t;
};

struct int2_t : fused::strong_typedef<int, fused::policy::lt<int>> {
  using self_t::self_t;
};

struct udt {
  udt() = default;
  explicit udt(int i) : value(i) {}
  int f() const { return value; }
  void g(int i) { value = i; }
  int value = 0;
};

struct udt_t : fused::strong_typedef<udt> {
  using self_t::self_t;
};

#include <memory>
struct uptr_t : fused::strong_typedef<std::unique_ptr<int>> {
  using self_t::self_t;
};

#include <archie/utils/test.h>

void canConstructStrongTypedef() {
  int_t i{4};
  ASSERT_EQ(4, static_cast<int>(i));
  udt_t u1;
  ASSERT_EQ(0, static_cast<udt>(u1).f());
  udt_t u2{3};
  ASSERT_EQ(3, static_cast<udt>(u2).f());
}

void canAccessUnderlyingTypeMembers() {
  udt_t u{3};
  ASSERT_EQ(3, u->f());
  u->g(4);
  ASSERT_EQ(4, u->f());
  ASSERT_EQ(4, u->value);
}

void canUseEqPolicy() {
  int_t i{3};
  ASSERT_TRUE(i == 3);
  ASSERT_TRUE(3 == i);
  ASSERT_FALSE(i != 3);
  ASSERT_FALSE(3 != i);
  ASSERT_FALSE(i == 4);
  ASSERT_FALSE(4 == i);
  ASSERT_TRUE(i != 4);
  ASSERT_TRUE(4 != i);
}

void canUseLtPolicy() {
  int2_t i{3};
  ASSERT_TRUE(i < 4);
  ASSERT_FALSE(i < 3);
  ASSERT_FALSE(3 > i);
  ASSERT_TRUE(4 > i);
}

int main() {
  canConstructStrongTypedef();
  canAccessUnderlyingTypeMembers();
  canUseEqPolicy();
  canUseLtPolicy();
  return 0;
}
