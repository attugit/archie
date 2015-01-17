#pragma once

namespace archie {
namespace utils {
  namespace meta {

    template <typename... Ts>
    struct inherit_all : Ts... {};
  }
}
}
