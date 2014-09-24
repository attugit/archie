#ifndef ARCHIE_UTILS_SELECT_H_INCLUDED
#define ARCHIE_UTILS_SELECT_H_INCLUDED

#include <archie/utils/unique_tuple.h>
#include <tuple>
#include <vector>

namespace archie {
namespace utils {

  template <typename... Args>
  struct Select {
    using SelfType = Select<Args...>;
    using Selection = UniqueTuple<Args&...>;

    template <typename... Types>
    static Selection from(UniqueTuple<Types...>& ut) {
      return std::tie(std::get<Args>(ut)...);
    }

    template <template <class...> class Container, typename... Types>
    static std::vector<Selection> from(
        Container<UniqueTuple<Types...>>& input) {
      std::vector<Selection> output;
      output.reserve(input.size());
      for (auto&& item : input) { output.emplace_back(SelfType::from(item)); }
      return output;
    }
  };
}
}

#endif
