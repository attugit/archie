#include <archie/utils/types/strong_typedef.h>
#include <archie/utils/types/policies.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/tuple_view.h>
#include <string>
#include <archie/utils/test.h>

namespace types = archie::utils::types;
namespace fused = archie::utils::fused;

namespace detail {
struct PackedStruct {
  struct Id : types::strong_typedef<Id, unsigned, types::policy::eq> {
    using self_t::self_t;
  };
  struct Name : types::strong_typedef<Name, std::string, types::policy::eq> {
    using self_t::self_t;
  };
  struct Value : types::strong_typedef<Value, int, types::policy::eq> {
    using self_t::self_t;
  };
  struct Amount : types::strong_typedef<Amount, unsigned> {
    using self_t::self_t;
  };

  using type = fused::tuple<Id, Name, Value, Amount>;
};
}

using PackedStruct = detail::PackedStruct::type;
using Id = detail::PackedStruct::Id;
using Name = detail::PackedStruct::Name;
using Value = detail::PackedStruct::Value;
using Amount = detail::PackedStruct::Amount;

void canCreate() {
  PackedStruct pack{Id{1u}, Name{"Aqq"}, Value{-20}, Amount{10u}};
  EXPECT_EQ(1u, fused::get<Id>(pack));
  EXPECT_EQ("Aqq", fused::get<Name>(pack));
  EXPECT_EQ(-20, fused::get<Value>(pack));
}

void canSelect() {
  PackedStruct pack{Id{1u}, Name{"Aqq"}, Value{-20}, Amount{10u}};
  auto select = fused::select<Id, Value>(pack);
  EXPECT_EQ(1u, fused::get<Id>(select));
  EXPECT_EQ(-20, fused::get<Value>(select));
}

int main() {
  canCreate();
  canSelect();
  return 0;
}
