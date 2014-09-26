#ifndef ARCHIE_UTILS_SELECT_H_INCLUDED
#define ARCHIE_UTILS_SELECT_H_INCLUDED

#include <archie/utils/unique_tuple.h>
#include <tuple>
#include <vector>
#include <functional>

namespace archie {
namespace utils {

  template <typename... Args>
  struct Select {
    using SelfType = Select<Args...>;
    using Selection = UniqueTuple<std::reference_wrapper<Args>...>;

    template <typename... Types>
    static Selection from(UniqueTuple<Types...>& ut) {
      return Selection(std::get<Args>(ut)...);
    }

    template <typename... Types>
    static UniqueTuple<Args...> copy(UniqueTuple<Types...>& ut) {
      return UniqueTuple<Args...>(std::get<Args>(ut)...);
    }

    // template <template <class...> class Container, typename... Types>
    // static Selection from(Container<UniqueTuple<Types...>>& input) {
    //  return SelfType::from(std::begin(input), std::end(input));
    //}

    template <typename Iterator>
    static std::vector<Selection> from(Iterator first, Iterator last) {
      std::vector<Selection> output;
      SelfType::insert(first, last, output);
      return output;
    }

    template <typename Iterator, typename Func>
    static std::vector<Selection> from_if(Iterator first, Iterator last,
                                          Func func) {
      std::vector<Selection> output;
      SelfType::insert_if(first, last, func, output);
      return output;
    }

    template <typename Iterator>
    static std::vector<UniqueTuple<Args...>> copy(Iterator first,
                                                  Iterator last) {
      std::vector<UniqueTuple<Args...>> output;
      SelfType::insert(first, last, output);
      return output;
    }

    template <typename Iterator, typename Func>
    static std::vector<UniqueTuple<Args...>> copy_if(Iterator first,
                                                     Iterator last, Func func) {
      std::vector<UniqueTuple<Args...>> output;
      SelfType::insert_if(first, last, func, output);
      return output;
    }

  private:
    template <typename Iterator, typename Output>
    static void insert(Iterator first, Iterator last, Output& output) {
      output.reserve(std::distance(first, last));
      while (first != last) {
        output.emplace_back(std::get<Args>(*first)...);
        ++first;
      }
    }

    template <typename Iterator, typename Func, typename Output>
    static void insert_if(Iterator first, Iterator last, Func func,
                          Output& output) {
      output.reserve(std::distance(first, last));
      while (first != last) {
        if (func(*first)) output.emplace_back(std::get<Args>(*first)...);
        ++first;
      }
      output.shrink_to_fit();
    }
  };
}
}

#endif
