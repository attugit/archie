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

using Table = TableType<std::vector, PersonId, PersonName, PersonAge>;
namespace au = archie::utils;
struct typeholder_test : ::testing::Test {};

template <typename T, typename Row>
typename T::value_type& select(Row& r) {
  return au::get<T>(r).template get<typename T::value_type>();
}

TEST_F(typeholder_test, nothing) {
  Table table;
  table.emplace_back(PersonId(1), PersonName("Name"), PersonAge(30));
  auto row = table.front();
  EXPECT_EQ(30, select<PersonAge>(row));
}
