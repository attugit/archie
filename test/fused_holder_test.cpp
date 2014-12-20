#include <archie/utils/fused/holder.h>
#include <test/assert.h>

namespace fused = archie::utils::fused;

struct type_ {
  type_() = default;
  template <typename Tp>
  explicit type_(Tp t)
      : v{t} {}
  unsigned v = 0;
};

bool operator==(type_ const& lhs, type_ const& rhs) { return lhs.v == rhs.v; }

void canCreateHolder() {
  auto h = fused::holder<type_>{};
  EXPECT_EQ(0u, static_cast<type_ const&>(h).v);
}

void canAssignValueToHolder() {
  auto h = fused::holder<type_>{3u};
  EXPECT_EQ(3u, static_cast<type_ const&>(h).v);
  h = type_{5u};
  EXPECT_EQ(5u, static_cast<type_ const&>(h).v);
}

int main() {
  canCreateHolder();
  return 0;
}
