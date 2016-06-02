#pragma once

#include <archie/meta/comparable.hpp>

namespace archie::meta
{
  template <typename... T>
  struct type_list : comparable<type_list<T...>> {
  };
}
