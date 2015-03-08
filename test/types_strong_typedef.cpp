#include <archie/utils/types/strong_typedef.h>
#include <archie/utils/types/policies.h>
#include <archie/utils/traits.h>
#include <type_traits>

namespace types = archie::utils::types;
namespace traits = archie::utils::traits;
namespace policy = types::policy;

struct int1_t : types::strong_typedef<int1_t, int, policy::eq> {
  using self_t::self_t;
};

struct int2_t : types::strong_typedef<int2_t, int, policy::lt> {
  using self_t::self_t;
};

struct int3_t : types::strong_typedef<int3_t, int, policy::add> {
  using self_t::self_t;
};

struct udt {
  udt() = default;
  explicit udt(int i) : value(i) {}
  int f() const { return value; }
  void g(int i) { value = i; }
  int value = 0;
};

struct udt_t : types::strong_typedef<udt_t, udt> {
  using self_t::self_t;
};

#include <memory>
struct uptr_t : types::strong_typedef<uptr_t, std::unique_ptr<int>> {
  using self_t::self_t;
};

#include <vector>
struct uvec_t : types::strong_typedef<uvec_t, std::vector<unsigned>, policy::iterate> {
  using self_t::self_t;
};

#include <archie/utils/test.h>

void canConstructStrongTypedef() {
  int1_t i{4};
  ASSERT_EQ(4, types::extract(i));
  udt_t u1;
  ASSERT_EQ(0, types::extract(u1).f());
  udt_t u2{3};
  ASSERT_EQ(3, types::extract(u2).f());
}

void canExtractValue() {
  int1_t i{4};
  ASSERT_EQ(4, types::extract(i));
  types::extract(i) = 7;
  ASSERT_EQ(7, types::extract(i));
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
  ASSERT_EQ(1, types::extract(i));
  i += 1;
  ASSERT_EQ(2, types::extract(i));
  i += 2;
  ASSERT_EQ(4, types::extract(i));

  auto x = i + 3;
  static_assert(std::is_same<decltype(i), decltype(x)>::value, "");
  ASSERT_EQ(4, types::extract(i));
  ASSERT_EQ(7, types::extract(x));
}

void canUseIteratePolicy() {
  uvec_t vec{0u, 1u, 2u, 3u, 4u};
  ASSERT_EQ(5u, vec->size());
  auto i = 5u;
  for (auto&& x : vec) {
    x += i;
    --i;
  }
  ASSERT_EQ(0u, i);
  for (auto const& x : vec) { ASSERT_EQ(5u, x); }
}

int main() {
  canConstructStrongTypedef();
  canExtractValue();
  canAccessUnderlyingTypeMembers();
  canUseEqPolicy();
  canUseLtPolicy();
  canUseAddPolicy();
  canUseIteratePolicy();
  return 0;
}
