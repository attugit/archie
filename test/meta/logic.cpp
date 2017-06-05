#include <archie/meta/logic.hpp>

namespace
{
  namespace meta = archie::meta;

  using true_t = meta::boolean<true>;
  using false_t = meta::boolean<false>;

  static_assert(meta::all<>);
  static_assert(meta::all<true_t>);
  static_assert(meta::all<true_t, true_t>);
  static_assert(!meta::all<false_t>);
  static_assert(!meta::all<false_t, true_t>);
  static_assert(!meta::all<true_t, false_t>);
  static_assert(!meta::all<false_t, false_t>);

  static_assert(!meta::any<>);
  static_assert(!meta::any<false_t>);
  static_assert(!meta::any<false_t, false_t>);
  static_assert(meta::any<true_t>);
  static_assert(meta::any<true_t, false_t>);
  static_assert(meta::any<false_t, true_t>);

  static_assert(meta::none<>);
  static_assert(!meta::none<true_t>);
  static_assert(!meta::none<true_t, true_t>);
  static_assert(meta::none<false_t>);
  static_assert(!meta::none<false_t, true_t>);
  static_assert(!meta::none<true_t, false_t>);
  static_assert(meta::none<false_t, false_t>);
}
