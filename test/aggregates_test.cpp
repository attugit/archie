#include <gtest/gtest.h>
#include <archie/utils/unique_tuple.h>
#include <archie/utils/holder.h>
#include <archie/utils/get.h>
#include <archie/utils/select.h>
#include <string>
#include <memory>
#include <list>

namespace detail {
struct PackedStruct {
  struct Id : archie::utils::Holder<unsigned> {
    using Holder::Holder;
  };
  struct Name : archie::utils::Holder<std::string> {
    using Holder::Holder;
  };
  struct Value : archie::utils::Holder<int> {
    using Holder::Holder;
  };
  struct Amount : archie::utils::Holder<unsigned> {
    using Holder::Holder;
  };

  using type = archie::utils::UniqueTuple<Id, Name, Value, Amount>;
};
}

struct PackedStruct : detail::PackedStruct::type {
  using Id = detail::PackedStruct::Id;
  using Name = detail::PackedStruct::Name;
  using Value = detail::PackedStruct::Value;
  using Amount = detail::PackedStruct::Amount;
  using BaseType = detail::PackedStruct::type;
  using BaseType::BaseType;
};

namespace au = archie::utils;
template <typename... Args>
struct Apply {
  template <typename Func, typename... Types>
  static auto call(Func func, au::UniqueTuple<Types...> const& ut) {
    return func(au::get<Args>(ut)...);
  }
};

namespace {

struct aggregates_test : ::testing::Test {
  std::unique_ptr<PackedStruct> pack;
};

TEST_F(aggregates_test, canCreate) {
  pack = std::make_unique<PackedStruct>(1, "Aqq", -20, 10);
  EXPECT_EQ(1, au::get<PackedStruct::Id>(*pack));
  EXPECT_EQ("Aqq", *au::get<PackedStruct::Name>(*pack));
  EXPECT_EQ(-20, au::get<PackedStruct::Value>(*pack));
  pack.reset();
}

TEST_F(aggregates_test, canSelect) {
  pack = std::make_unique<PackedStruct>(1, "Aqq", -20, 10);
  auto select = au::Select<PackedStruct::Id, PackedStruct::Value>::from(*pack);
  EXPECT_EQ(1, au::get<PackedStruct::Id>(select));
  EXPECT_EQ(-20, au::get<PackedStruct::Value>(select));
}

TEST_F(aggregates_test, canSelectCollection) {
  std::list<PackedStruct> collection;
  collection.emplace_back(1, "Aqq", -20, 10);
  collection.emplace_back(2, "Bqq", -10, 20);
  collection.emplace_back(3, "Cqq", -30, 400);

  auto select = au::Select<PackedStruct::Id>::from(std::begin(collection),
                                                   std::end(collection));
  ASSERT_EQ(3u, select.size());
  EXPECT_EQ(1, au::get<PackedStruct::Id>(select[0]));
  EXPECT_EQ(2, au::get<PackedStruct::Id>(select[1]));
  EXPECT_EQ(3, au::get<PackedStruct::Id>(select[2]));
}

TEST_F(aggregates_test, canSelectCollectionIf) {
  std::list<PackedStruct> collection;
  collection.emplace_back(1, "Aqq", -20, 10);
  collection.emplace_back(2, "Bqq", -10, 20);
  collection.emplace_back(3, "Cqq", -30, 400);

  auto isOdd =
      [](auto&& item) { return (*au::get<PackedStruct::Id>(item) % 2) != 0; };
  auto select = au::Select<PackedStruct::Id>::from_if(
      std::begin(collection), std::end(collection), isOdd);
  ASSERT_EQ(2u, select.size());
  EXPECT_EQ(1, au::get<PackedStruct::Id>(select[0]));
  EXPECT_EQ(3, au::get<PackedStruct::Id>(select[1]));
}

TEST_F(aggregates_test, canApply) {
  PackedStruct pack(1, "Aqq", -20, 10);
  auto func = [](auto&& value) { EXPECT_TRUE(value < 0); };
  Apply<PackedStruct::Value>::call(func, pack);
}
}
