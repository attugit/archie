#include <archie/utils/traits.h>

#include <vector>
#include <utility>
#include <memory>

namespace traits = archie::utils::traits;

// using vec_t = std::vector<int>;
// using pair_t = std::pair<int, int>;
// using ptr_t = std::unique_ptr<int>;

// struct TypeWithPublicData {
//  int data;
//};

// struct TypeWithPrivateData {
// private:
//  int data;
//};

// static_assert(au::has_value_type<vec_t>::value, "");
// static_assert(!au::has_value_type<pair_t>::value, "");
// static_assert(!au::has_value_type<int>::value, "");

// static_assert(au::has_reserve<vec_t>::value, "");
// static_assert(!au::has_reserve<pair_t>::value, "");
// static_assert(!au::has_reserve<int>::value, "");

// static_assert(au::has_data_property<TypeWithPublicData>::value, "");
// static_assert(!au::has_data_property<vec_t>::value, "");
// static_assert(!au::has_data_property<TypeWithPrivateData>::value, "");
// static_assert(!au::has_data_property<int>::value, "");

// static_assert(au::is_copy_assignable<vec_t>::value, "");
// static_assert(au::is_copy_assignable<pair_t>::value, "");
// static_assert(au::is_copy_assignable<int>::value, "");
// static_assert(!au::is_copy_assignable<ptr_t>::value, "");

namespace {
struct callable {
  void operator()(unsigned&, double&, char&) {}
  void operator()(const unsigned&, const char&) {}
  void operator()(unsigned) {}
};

static_assert(!traits::is_callable<callable, unsigned, double, char>::value,
              "");
static_assert(!traits::is_callable<callable, const unsigned&, const double&,
                                   const char&>::value,
              "");
static_assert(
    !traits::is_callable<callable, unsigned&, const double&, char&>::value, "");
static_assert(traits::is_callable<callable, unsigned&, double&, char&>::value,
              "");
static_assert(
    traits::is_callable<callable, const unsigned&, const char&>::value, "");
static_assert(!traits::is_callable<callable, double, std::string&>::value, "");
static_assert(traits::is_callable<callable, unsigned>::value, "");
static_assert(!traits::is_callable<callable, std::string>::value, "");
}
