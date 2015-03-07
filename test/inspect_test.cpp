#include <archie/utils/traits/has_member.h>

#include <vector>
#include <utility>
#include <memory>

namespace {

namespace au = archie::utils;
namespace meta = au::meta;
namespace traits = au::traits;

using vec_t = std::vector<int>;
using pair_t = std::pair<int, int>;
using ptr_t = std::unique_ptr<int>;

template <typename Tp>
using InspectReserve = decltype(
    std::declval<Tp>().reserve(std::declval<typename Tp::size_type>()));

template <typename Tp>
using InspectDataProperty = decltype(std::declval<Tp>().data);

template <typename Tp>
using has_reserve = traits::has_member<InspectReserve, meta::type_list<Tp>>;

template <typename Tp>
using ValueType = typename Tp::value_type;

template <typename Tp>
using has_value_type = traits::has_member<ValueType, meta::type_list<Tp>>;

template <typename Tp>
using has_data_property =
    traits::has_member<InspectDataProperty, meta::type_list<Tp>>;

struct TypeWithPublicData {
  int data;
};

struct TypeWithPrivateData {
private:
  int data;
};

static_assert(has_value_type<vec_t>::value, "");
static_assert(!has_value_type<pair_t>::value, "");
static_assert(!has_value_type<int>::value, "");

static_assert(has_reserve<vec_t>::value, "");
static_assert(!has_reserve<pair_t>::value, "");
static_assert(!has_reserve<int>::value, "");

static_assert(has_data_property<TypeWithPublicData>::value, "");
static_assert(!has_data_property<vec_t>::value, "");
static_assert(!has_data_property<TypeWithPrivateData>::value, "");
static_assert(!has_data_property<int>::value, "");
}

int main() { return 0; }
