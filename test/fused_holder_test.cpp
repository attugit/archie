#include <archie/utils/fused/holder.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;
namespace traits = archie::utils::traits;

struct type_ {
  type_() = default;
  template <typename Tp>
  explicit type_(Tp t)
      : v{t} {}

  unsigned v = 0;
};

bool operator==(type_ const& lhs, type_ const& rhs) { return lhs.v == rhs.v; }
bool operator<(type_ const& lhs, type_ const& rhs) { return lhs.v < rhs.v; }

void canCreateHolder() {
  auto h = fused::holder<type_>{};
  EXPECT_EQ(0u, h.get().v);
}

void canAssignValueToHolder() {
  static_assert(
      traits::is_equality_comparable<fused::holder<type_>, type_>::value, "");
  static_assert(
      traits::is_less_than_comparable<fused::holder<type_>, type_>::value, "");
  auto h = fused::holder<type_>{3u};
  auto t = type_{5u};
  EXPECT_EQ(3u, h.get().v);
  h = t;
  EXPECT_EQ(5u, h.get().v);
  EXPECT_EQ(h, t);
  t.v = 6u;
  EXPECT_TRUE(h < t);
  h = std::move(t);
  EXPECT_EQ(6u, h.get().v);
}

void canEqCompareHolders() {
  static_assert(traits::is_equality_comparable<fused::holder<type_>,
                                               fused::holder<type_>>::value,
                "");
  static_assert(
      traits::is_equality_comparable<fused::holder<type_>, type_>::value, "");

  auto a = fused::holder<type_>{1u};
  auto b = fused::holder<type_>{1u};
  auto c = fused::holder<type_>{2u};
  auto d = type_{1u};
  auto e = type_{0u};

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a == d);
  EXPECT_FALSE(a == e);
}

void canNeCompareHolders() {
  auto a = fused::holder<type_>{1u};
  auto b = fused::holder<type_>{1u};
  auto c = fused::holder<type_>{2u};
  auto d = type_{1u};
  auto e = type_{0u};

  EXPECT_FALSE(a != b);
  EXPECT_TRUE(a != c);
  EXPECT_FALSE(a != d);
  EXPECT_TRUE(a != e);
}

int main() {
  canCreateHolder();
  canAssignValueToHolder();
  canEqCompareHolders();
  canNeCompareHolders();
  return 0;
}
