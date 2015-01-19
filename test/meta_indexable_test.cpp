#include <archie/utils/meta/indexable.h>

namespace meta = archie::utils::meta;

template <std::size_t... ids>
struct with_index {
  static const auto value = sizeof...(ids);
};

namespace {
static_assert(meta::indexable_t<with_index, 0>::value == 0, "");
static_assert(meta::indexable_t<with_index, 1>::value == 1, "");
static_assert(meta::indexable_t<with_index, 2>::value == 2, "");
static_assert(meta::indexable_t<with_index, 3>::value == 3, "");
}

int main() { return 0; }
