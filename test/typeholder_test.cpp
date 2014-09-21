#include <archie/utils/typeholder.h>
#include <archie/utils/select.h>
#include <string>

ARCHIE_UTILS_GENERATE_TYPE_HOLDER(PersonId, unsigned);
ARCHIE_UTILS_GENERATE_TYPE_HOLDER(PersonName, std::string);
ARCHIE_UTILS_GENERATE_TYPE_HOLDER(PersonAge, unsigned);

#include <archie/utils/typeset.h>

template <typename... Columns>
using RowType = archie::utils::TypeSet<Columns...>;

template <template <class...> class Container, typename... Columns>
using TableType = Container<RowType<Columns...>>;

#include <gtest/gtest.h>
#include <vector>

using Table = TableType<std::vector, PersonId, PersonName, PersonAge>;
namespace au = archie::utils;
struct typeholder_test : ::testing::Test {};

TEST_F(typeholder_test, nothing) {
  Table table;
  table.emplace_back(PersonId(1), PersonName("Name"), PersonAge(30));
  auto row = table.front();
  EXPECT_EQ(30, au::select<PersonAge>(row));
  static_assert(au::is_TypeHolder<PersonId>::value, "");
  static_assert(au::is_less_comparable<PersonId, PersonId>::value, "");
  static_assert(au::is_less_comparable<PersonId>::value, "");
  static_assert(au::is_equality_comparable<PersonId, PersonId>::value, "");
  static_assert(au::is_equality_comparable<PersonId>::value, "");
  PersonId id1(1), id2(2);

  EXPECT_TRUE(id1 < id2);
  EXPECT_TRUE(id2 > id1);
  EXPECT_TRUE(id1 <= id2);
  EXPECT_TRUE(id2 >= id1);
  EXPECT_TRUE(id1 == id1);
  EXPECT_TRUE(id1 != id2);

  EXPECT_FALSE(id2 < id1);
  EXPECT_FALSE(id1 > id2);
  EXPECT_FALSE(id2 <= id1);
  EXPECT_FALSE(id1 >= id2);
  EXPECT_FALSE(id1 == id2);
  EXPECT_FALSE(id2 != id2);
}
