#include <archie/utils/fused/holder.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/tuple_view.h>
#include <string>
#include <archie/utils/test.h>

namespace detail {
struct PackedStruct {
  struct Id : archie::utils::fused::holder<unsigned> {
    using holder::holder;
  };
  struct Name : archie::utils::fused::holder<std::string> {
    using holder::holder;
  };
  struct Value : archie::utils::fused::holder<int> {
    using holder::holder;
  };
  struct Amount : archie::utils::fused::holder<unsigned> {
    using holder::holder;
  };

  using type = archie::utils::fused::tuple<Id, Name, Value, Amount>;
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

namespace fused = archie::utils::fused;

void canCreate() {
  PackedStruct pack{1, "Aqq", -20, 10};
  EXPECT_EQ(1u, fused::get<PackedStruct::Id>(pack));
  EXPECT_EQ("Aqq", fused::get<PackedStruct::Name>(pack));
  EXPECT_EQ(-20, fused::get<PackedStruct::Value>(pack));
}

void canSelect() {
  PackedStruct pack{1, "Aqq", -20, 10};
  auto select = fused::select<PackedStruct::Id, PackedStruct::Value>(pack);
  EXPECT_EQ(1u, fused::get<PackedStruct::Id>(select));
  EXPECT_EQ(-20, fused::get<PackedStruct::Value>(select));
}

int main() {
  canCreate();
  canSelect();
  return 0;
}
