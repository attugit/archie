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

int main() {
  canCreateHolder();
  canAssignValueToHolder();
  return 0;
}
