#include <archie/utils/fused/holder.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/tuple_view.h>
#include <string>
#include <archie/utils/test.h>

namespace detail {
struct PackedStruct {
  struct Id : archie::utils::fused::holder<unsigned> {
    using holder::holder;
    Id(Id const&) = default;
    Id(Id&) = default;
  };
  struct Name : archie::utils::fused::holder<std::string> {
    using holder::holder;
    Name(Name const&) = default;
    Name(Name&) = default;
  };
  struct Value : archie::utils::fused::holder<int> {
    using holder::holder;
    Value(Value const&) = default;
    Value(Value&) = default;
  };
  struct Amount : archie::utils::fused::holder<unsigned> {
    using holder::holder;
    Amount(Amount const&) = default;
    Amount(Amount&) = default;
  };

  using type = archie::utils::fused::tuple<Id, Name, Value, Amount>;
};
}

using PackedStruct = detail::PackedStruct::type;
using Id = detail::PackedStruct::Id;
using Name = detail::PackedStruct::Name;
using Value = detail::PackedStruct::Value;
using Amount = detail::PackedStruct::Amount;
using BaseType = detail::PackedStruct::type;

namespace fused = archie::utils::fused;

void canCreate() {
  PackedStruct pack{1, "Aqq", -20, 10};
  EXPECT_EQ(1u, fused::get<Id>(pack));
  EXPECT_EQ("Aqq", fused::get<Name>(pack));
  EXPECT_EQ(-20, fused::get<Value>(pack));
}

void canSelect() {
  PackedStruct pack{1, "Aqq", -20, 10};
  auto select = fused::select<Id, Value>(pack);
  EXPECT_EQ(1u, fused::get<Id>(select));
  EXPECT_EQ(-20, fused::get<Value>(select));
}

int main() {
  canCreate();
  canSelect();
  return 0;
}
