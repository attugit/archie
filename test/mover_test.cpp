#include <archie/fused/mover.h>

#include <type_traits>

namespace {
namespace fused = archie::fused;

struct empty {};
struct def {
  def(def&&) = default;
  def(def const&) = default;
};

struct nondef {
  nondef(nondef&&) {}
  nondef(nondef const&) {}
};

struct noncopyable {
  noncopyable(noncopyable&&) = default;
  noncopyable(noncopyable const&) = delete;
};

static_assert(std::is_same<fused::move_t<int>, int>::value, "");
static_assert(std::is_same<fused::move_t<empty>, empty>::value, "");
static_assert(std::is_same<fused::move_t<def>, def>::value, "");
static_assert(std::is_same<fused::move_t<nondef>, fused::detail::move_capture<nondef>>::value, "");
static_assert(
    std::is_same<fused::move_t<noncopyable>, fused::detail::move_capture<noncopyable>>::value,
    "");
}

#include <archie/test.h>

void canCastToConstValue() {
  auto i = 1;
  auto m = fused::move_t<const int&>(i);
  auto const& x = static_cast<const int&>(m);
  EXPECT_EQ(&i, &x);
}

int main() {
  canCastToConstValue();
  return 0;
}
