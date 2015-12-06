#include <archie/meta/logic.h>
#include <catch.hpp>

namespace {
namespace meta = archie::meta;

using true_t = meta::boolean<true>;
using false_t = meta::boolean<false>;

TEST_CASE("meta::logic") {
  static_assert(meta::all<>::value, "");
  static_assert(meta::all<true_t>::value, "");
  static_assert(meta::all<true_t, true_t>::value, "");
  static_assert(!meta::all<false_t>::value, "");
  static_assert(!meta::all<false_t, true_t>::value, "");
  static_assert(!meta::all<true_t, false_t>::value, "");
  static_assert(!meta::all<false_t, false_t>::value, "");

  static_assert(!meta::any<>::value, "");
  static_assert(!meta::any<false_t>::value, "");
  static_assert(!meta::any<false_t, false_t>::value, "");
  static_assert(meta::any<true_t>::value, "");
  static_assert(meta::any<true_t, false_t>::value, "");
  static_assert(meta::any<false_t, true_t>::value, "");

  static_assert(meta::none<>::value, "");
  static_assert(!meta::none<true_t>::value, "");
  static_assert(!meta::none<true_t, true_t>::value, "");
  static_assert(meta::none<false_t>::value, "");
  static_assert(!meta::none<false_t, true_t>::value, "");
  static_assert(!meta::none<true_t, false_t>::value, "");
  static_assert(meta::none<false_t, false_t>::value, "");
}
}
