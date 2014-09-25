#include <gtest/gtest.h>
#include <archie/utils/unique_tuple.h>
#include <archie/utils/typeholder.h>
#include <archie/utils/get.h>
#include <string>

namespace detail {
struct PackedStruct {
  struct Id : archie::utils::TypeHolder<unsigned> {
    using TypeHolder::TypeHolder;
  };
  struct Name : archie::utils::TypeHolder<std::string> {
    using TypeHolder::TypeHolder;
  };
  struct Value : archie::utils::TypeHolder<int> {
    using TypeHolder::TypeHolder;
  };
  struct Amount : archie::utils::TypeHolder<unsigned> {
    using TypeHolder::TypeHolder;
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

namespace {

struct aggregates_test : ::testing::Test {};

TEST_F(aggregates_test, nothing) {
  PackedStruct pack(1, "Aqq", -20, 10);
  EXPECT_EQ(1, au::get<PackedStruct::Id>(pack));
  EXPECT_EQ("Aqq", *au::get<PackedStruct::Name>(pack));
  EXPECT_EQ(-20, au::get<PackedStruct::Value>(pack));
}
}
