#pragma once

namespace archie {
namespace meta {
  template <typename... Ts>
  struct inherit_all : Ts... {};
}
}
