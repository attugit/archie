#include <archie/utils/fused/holder.h>
#include <memory>

namespace fused = archie::utils::fused;

struct type_ {};

bool operator==(type_ const& lhs, type_ const& rhs) { return &lhs == &rhs; }

GENERATE_HOLDER(TypeHolder, type_);

void canCreateHolder() {
  auto h1 = TypeHolder{};
  auto h2 = fused::holder<std::unique_ptr<type_>>{};
  (void)h1;
  (void)h2;
}

int main() {
  canCreateHolder();
  return 0;
}
