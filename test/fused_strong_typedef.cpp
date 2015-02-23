#include <archie/utils/fused/strong_typedef.h>
#include <archie/utils/fused/policies.h>
#include <archie/utils/traits.h>
#include <type_traits>

namespace fused = archie::utils::fused;
namespace traits = archie::utils::traits;
namespace policy = fused::policy;

struct int1_t : fused::strong_typedef<int1_t, int, policy::eq<int>> {
  using self_t::self_t;
};

struct int2_t : fused::strong_typedef<int2_t, int, policy::lt<int>> {
  using self_t::self_t;
};

struct int3_t : fused::strong_typedef<int3_t, int, policy::add<int>> {
  using self_t::self_t;
};

struct udt {
  udt() = default;
  explicit udt(int i) : value(i) {}
  int f() const { return value; }
  void g(int i) { value = i; }
  int value = 0;
};

struct udt_t : fused::strong_typedef<udt_t, udt> {
  using self_t::self_t;
};

#include <memory>
struct uptr_t : fused::strong_typedef<uptr_t, std::unique_ptr<int>> {
  using self_t::self_t;
};

#include <archie/utils/test.h>

void canConstructStrongTypedef() {
  int1_t i{4};
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
  int1_t i{3};
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
  ASSERT_FALSE(i < 2);

  ASSERT_FALSE(4 < i);
  ASSERT_FALSE(3 < i);
  ASSERT_TRUE(2 < i);

  ASSERT_TRUE(4 > i);
  ASSERT_FALSE(3 > i);
  ASSERT_FALSE(2 > i);

  ASSERT_FALSE(i > 4);
  ASSERT_FALSE(i > 3);
  ASSERT_TRUE(i > 2);

  ASSERT_TRUE(i <= 4);
  ASSERT_TRUE(i <= 3);
  ASSERT_FALSE(i <= 2);

  ASSERT_FALSE(4 <= i);
  ASSERT_TRUE(3 <= i);
  ASSERT_TRUE(2 <= i);

  ASSERT_FALSE(i >= 4);
  ASSERT_TRUE(i >= 3);
  ASSERT_TRUE(i >= 2);

  ASSERT_TRUE(4 >= i);
  ASSERT_TRUE(3 >= i);
  ASSERT_FALSE(2 >= i);
}

void canUseAddPolicy() {
  int3_t i{1};
  ASSERT_EQ(1, static_cast<int>(i));
  i += 1;
  ASSERT_EQ(2, static_cast<int>(i));
  i += 2;
  ASSERT_EQ(4, static_cast<int>(i));

  auto x = i + 3;
  static_assert(std::is_same<decltype(i), decltype(x)>::value, "");
  ASSERT_EQ(4, static_cast<int>(i));
  ASSERT_EQ(7, static_cast<int>(x));
}

int main() {
  canConstructStrongTypedef();
  canAccessUnderlyingTypeMembers();
  canUseEqPolicy();
  canUseLtPolicy();
  canUseAddPolicy();
  return 0;
}
