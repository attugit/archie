#include <archie/utils/traits.h>

#include <vector>
#include <utility>
#include <memory>

namespace traits = archie::utils::traits;

using vec_t = std::vector<int>;
using pair_t = std::pair<int, int>;
using ptr_t = std::unique_ptr<int>;

struct tester {
  tester& operator=(tester&&) = delete;
  tester& operator=(tester const&) = default;
  bool operator==(tester const&) const;
  bool operator==(int) const;
};

static_assert(traits::is_copy_assignable<vec_t>::value, "");
static_assert(traits::is_copy_assignable<pair_t>::value, "");
static_assert(traits::is_copy_assignable<int>::value, "");
static_assert(!traits::is_copy_assignable<ptr_t>::value, "");
static_assert(traits::is_assignable<ptr_t, decltype(nullptr)>::value, "");
static_assert(traits::is_move_assignable<ptr_t>::value, "");
static_assert(traits::is_copy_assignable<tester>::value, "");
static_assert(!traits::is_move_assignable<tester>::value, "");
static_assert(traits::is_equality_comparable<tester, tester>::value, "");
static_assert(traits::is_equality_comparable<tester, int>::value, "");
static_assert(!traits::is_equality_comparable<tester, ptr_t>::value, "");

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

int main() { return 0; }
