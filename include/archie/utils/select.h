#ifndef ARCHIE_UTILS_SELECT_H_INCLUDED
#define ARCHIE_UTILS_SELECT_H_INCLUDED

#include <archie/utils/unique_tuple.h>
#include <archie/utils/get.h>
#include <vector>
#include <functional>

namespace archie {
namespace utils {

  template <typename... Args>
  struct Select {
    using SelfType = Select<Args...>;
    using RefType = Select<std::reference_wrapper<Args>...>;
    using Selection = UniqueTuple<Args...>;

    template <typename... Types>
    static auto from(UniqueTuple<Types...>& ut) {
      return RefType::copy(ut);
    }

    template <typename... Types>
    static Selection copy(UniqueTuple<Types...>& ut) {
      return Selection{archie::utils::get<Args>(ut)...};
    }

    template <typename Iterator>
    static auto from(Iterator first, Iterator last) {
      return RefType::copy(first, last);
    }

    template <typename Iterator, typename Func>
    static auto from_if(Iterator first, Iterator last, Func func) {
      return RefType::copy_if(first, last, func);
    }

    template <typename Iterator>
    static std::vector<Selection> copy(Iterator first, Iterator last) {
      std::vector<Selection> output;
      output.reserve(std::distance(first, last));
      while (first != last) {
        output.emplace_back(archie::utils::get<Args>(*first)...);
        ++first;
      }
      return output;
    }

    template <typename Iterator, typename Func>
    static std::vector<Selection> copy_if(Iterator first, Iterator last,
                                          Func func) {
      std::vector<Selection> output;
      output.reserve(std::distance(first, last));
      while (first != last) {
        if (func(*first))
          output.emplace_back(archie::utils::get<Args>(*first)...);
        ++first;
      }
      output.shrink_to_fit();
      return output;
    }
  };
}
}

#endif
