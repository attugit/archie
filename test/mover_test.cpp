#include <archie/utils/mover.h>

#include <type_traits>

namespace {
namespace au = archie::utils;

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

static_assert(std::is_same<au::move_t<int>, int>::value, "");
static_assert(std::is_same<au::move_t<empty>, empty>::value, "");
static_assert(std::is_same<au::move_t<def>, def>::value, "");
static_assert(std::is_same<au::move_t<nondef>, au::mover<nondef>>::value, "");
static_assert(
    std::is_same<au::move_t<noncopyable>, au::mover<noncopyable>>::value, "");
}
