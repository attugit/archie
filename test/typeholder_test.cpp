#include <archie/utils/typeholder.h>
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
#include <list>
#include <archie/utils/select.h>
#include <archie/utils/get.h>

using Table = TableType<std::list, PersonId, PersonName, PersonAge>;
namespace au = archie::utils;
struct typeholder_test : ::testing::Test {};

TEST_F(typeholder_test, nothing) {
  Table table;
  table.emplace_back(1, "Name", 30);
  table.emplace_back(2, "Other", 40);
  auto& row = table.front();
  auto sel = au::Select<PersonId, PersonAge>::from(row);
  EXPECT_EQ(1, *au::get<PersonId>(sel));
  au::get<PersonId>(sel) = 7;
  EXPECT_EQ(7, *au::get<PersonId>(sel));
  EXPECT_EQ(7, *au::get<PersonId>(row));

  auto sel2 = au::Select<PersonAge, PersonId>::from(table);
  EXPECT_EQ(2, sel2.size());

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
