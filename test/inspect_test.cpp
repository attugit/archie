#include <archie/utils/inspect.h>

#include <vector>
#include <utility>
#include <memory>

namespace au = archie::utils;

using vec_t = std::vector<int>;
using pair_t = std::pair<int, int>;
using ptr_t = std::unique_ptr<int>;

struct TypeWithPublicData {
  int data;
};

struct TypeWithPrivateData {
private:
  int data;
};

static_assert(au::has_value_type<vec_t>::value, "");
static_assert(!au::has_value_type<pair_t>::value, "");
static_assert(!au::has_value_type<int>::value, "");

static_assert(au::has_reserve<vec_t>::value, "");
static_assert(!au::has_reserve<pair_t>::value, "");
static_assert(!au::has_reserve<int>::value, "");

static_assert(au::has_data_property<TypeWithPublicData>::value, "");
static_assert(!au::has_data_property<vec_t>::value, "");
static_assert(!au::has_data_property<TypeWithPrivateData>::value, "");
static_assert(!au::has_data_property<int>::value, "");

static_assert(au::is_copy_assignable<vec_t>::value, "");
static_assert(au::is_copy_assignable<pair_t>::value, "");
// static_assert(au::is_copy_assignable<int>::value, "");
static_assert(!au::is_copy_assignable<ptr_t>::value, "");

// using int_t = decltype(std::declval<int>() = std::declval<int const&>());
