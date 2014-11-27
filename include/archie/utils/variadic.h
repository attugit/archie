#ifndef ARCHIE_UTILS_VARIADIC_H_INCLUDED
#define ARCHIE_UTILS_VARIADIC_H_INCLUDED

#include <utility>
#include <archie/utils/inspect.h>
#include <archie/utils/for_each.h>
#include <archie/utils/meta/apply.h>
#include <archie/utils/inherit_all.h>
#include <archie/utils/number.h>

namespace archie {
namespace utils {

  template <typename... Ts>
  struct Variadic {
  public:
    static constexpr decltype(auto) size() { return sizeof...(Ts); }

  private:
    template <typename Tp, std::size_t Idx>
    struct IndexedType {};

    template <std::size_t... Indexes>
    static constexpr decltype(auto) make_indexed_variadic(
        std::index_sequence<Indexes...>) {
      return InheritAll<IndexedType<Ts, Indexes>...>{};
    }

    template <typename Tp, std::size_t Idx>
    static constexpr decltype(auto) indexed(IndexedType<Tp, Idx> const&) {
      return meta::number<Idx>{};
    }

  public:
    template <typename Tp>
    static constexpr decltype(auto) index_of() {
      using sequence = std::make_index_sequence<size()>;
      using indicies =
          decltype(make_indexed_variadic(std::declval<sequence>()));
      using type = decltype(indexed<Tp>(std::declval<indicies>()));
      return type::value;
    }

    template <template <typename...> class MetaFunction>
    using Apply = MetaFunction<Ts...>;
  };
}
}

#endif
