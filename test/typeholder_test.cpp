#include <archie/utils/holder.h>
#include <string>

ARCHIE_UTILS_GENERATE_HOLDER(PersonId, unsigned);
ARCHIE_UTILS_GENERATE_HOLDER(PersonName, std::string);
ARCHIE_UTILS_GENERATE_HOLDER(PersonAge, unsigned);

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
#include <algorithm>

using Table = TableType<std::vector, PersonId, PersonName, PersonAge>;
namespace au = archie::utils;
struct typeholder_test : ::testing::Test {};

TEST_F(typeholder_test, nothing) {
  Table table;
  table.emplace_back(1, "Name", 30);
  table.emplace_back(2, "Other", 40);
  table.emplace_back(3, "Another", 50);
  auto& row = table.front();
  auto sel = au::Select<PersonId, PersonAge>::from(row);
  EXPECT_EQ(1, au::get<PersonId>(sel));
  au::get<PersonAge>(sel) = 60;
  EXPECT_EQ(60, au::get<PersonAge>(sel));
  EXPECT_EQ(60, au::get<PersonAge>(row));

  auto sel2 =
      au::Select<PersonAge, PersonId>::from(std::begin(table), std::end(table));
  EXPECT_EQ(3u, sel2.size());

  auto byAge = [](auto lhs, auto rhs) {
    return au::get<PersonAge>(lhs) < au::get<PersonAge>(rhs);
  };

  std::sort(std::begin(sel2), std::end(sel2), byAge);
  EXPECT_EQ(40, au::get<PersonAge>(sel2[0]));
  EXPECT_EQ(50, au::get<PersonAge>(sel2[1]));
  EXPECT_EQ(60, au::get<PersonAge>(sel2[2]));

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
