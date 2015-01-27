#include <archie/utils/fused/mover.h>

#include <type_traits>

namespace {
namespace fused = archie::utils::fused;

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
static_assert(std::is_same<fused::move_t<nondef>,
                           fused::detail::move_capture<nondef>>::value,
              "");
static_assert(std::is_same<fused::move_t<noncopyable>,
                           fused::detail::move_capture<noncopyable>>::value,
              "");
}

int main() { return 0; }
