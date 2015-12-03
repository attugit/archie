#include <utility>
#include <archie/container/ring_adapter.hpp>

#include <algorithm>
#include <catch.hpp>
#include <resource.hpp>
namespace {
using namespace archie;
TEST_CASE("ring_adapter vector", "[ring]") {
  using ring_t = ring_adapter<std::vector<test::resource>>;
  ring_t ring;
  ring->reserve(4);
  REQUIRE(ring.empty());
  REQUIRE(ring.size() == 0);
  REQUIRE(std::distance(std::begin(ring), std::end(ring)) == ring.size());
  auto const capacity = static_cast<test::resource::value_type>(ring->capacity());
  auto const salt = 42;
  auto const check = [](ring_t& ra, auto item, typename ring_t::size_type size) {
    ra.emplace_back(item);
    REQUIRE(!ra.empty());
    REQUIRE((ra.size() == size));
    auto const distance =
        static_cast<typename ring_t::size_type>(std::distance(std::begin(ra), std::end(ra)));
    REQUIRE((distance == size));
    REQUIRE((*(std::begin(ra) + ra.size() - 1) == item));
  };

  for (auto idx = 0; idx < capacity; ++idx) {
    check(ring, idx + salt, static_cast<typename ring_t::size_type>(idx + 1));
  }
  for (auto idx = 0; idx < capacity; ++idx) {
    check(ring, idx + capacity + salt, static_cast<typename ring_t::size_type>(capacity));
  }
}
}
