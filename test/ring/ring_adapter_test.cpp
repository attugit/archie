#include <utility>
#include <archie/container/ring_adapter.hpp>

#include <algorithm>
#include <gtest/gtest.h>
#include <resource.hpp>
namespace {
using namespace archie;
TEST(ring_adapter, vector)
{
  using ring_t = ring_adapter<std::vector<test::resource>>;
  ring_t ring;
  ring->reserve(4);
  EXPECT_TRUE(ring.empty());
  EXPECT_EQ(ring.size(), 0u);
  EXPECT_EQ(static_cast<std::size_t>(std::distance(std::begin(ring), std::end(ring))), ring.size());
  auto const capacity = static_cast<test::resource::value_type>(ring->capacity());
  auto const salt = 42;
  auto const check = [](ring_t& ra, auto item, typename ring_t::size_type size) {
    ra.emplace_back(item);
    EXPECT_FALSE(ra.empty());
    EXPECT_EQ(ra.size(), size);
    auto const distance =
        static_cast<typename ring_t::size_type>(std::distance(std::begin(ra), std::end(ra)));
    EXPECT_EQ(distance, size);
    EXPECT_EQ(*(std::begin(ra) + ra.size() - 1), item);
  };

  for (auto idx = 0; idx < capacity; ++idx) {
    check(ring, idx + salt, static_cast<typename ring_t::size_type>(idx + 1));
  }
  for (auto idx = 0; idx < capacity; ++idx) {
    check(ring, idx + capacity + salt, static_cast<typename ring_t::size_type>(capacity));
  }
}
}
