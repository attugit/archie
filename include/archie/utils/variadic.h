#ifndef ARCHIE_UTILS_VARIADIC_H_INCLUDED
#define ARCHIE_UTILS_VARIADIC_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {

  template <typename Tp>
  using Alias = Tp;

  template <typename Func, typename... Args>
  constexpr void for_each(Func&& func, Args&&... args) {
    (void)Alias<int[]>{(func(std::forward<Args>(args)), 1)...};
  }

  template <typename... Ts>
  struct InheritAll : Ts... {};

  template <typename Tp, std::size_t Idx>
  struct IndexedType {};

  template <typename... Ts>
  struct Variadic {
  public:
    static constexpr decltype(auto) size() { return sizeof...(Ts); }

  private:
    template <std::size_t... Indexes>
    static constexpr decltype(auto) make_indexed_variadic(
        std::index_sequence<Indexes...>) {
      return InheritAll<IndexedType<Ts, Indexes>...>{};
    }

    template <typename Tp, std::size_t Idx>
    static constexpr decltype(auto) indexed(IndexedType<Tp, Idx> const&) {
      return std::integral_constant<std::size_t, Idx>{};
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
  };

  template <typename... Ts>
  struct Tuple {
  private:
    template <typename... Args>
    constexpr static decltype(auto) make_storage(Args&&... args) {
      return [args...](auto&& func) -> decltype(auto) {
        return std::forward<decltype(func)>(func)(args...);
      };
    }
    using Storage = decltype(make_storage(std::declval<Ts>()...));
    Storage storage;

  public:
    using variadic = Variadic<Ts...>;

    template <typename... Up>
    constexpr explicit Tuple(Up&&... u)
        : storage(make_storage(std::forward<Up>(u)...)) {}

    constexpr Tuple() : Tuple(Ts {}...) {}

    constexpr decltype(auto) size() const { return variadic::size(); }

    template <typename Func>
    decltype(auto) apply(Func&& func) {
      return storage(std::forward<Func>(func));
    }
  };

  namespace detail {
    struct at_impl {
    private:
      template <std::size_t Tp>
      struct wrapper {
        template <typename Up>
        constexpr wrapper(Up&&) {}
      };

    public:
      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct get;

      template <std::size_t n, std::size_t... ignore>
      struct get<n, std::index_sequence<ignore...>> {
        template <typename Nth, typename...>
        constexpr decltype(auto) operator()(wrapper<ignore>..., Nth&& nth,
                                            ...) const {
          return std::forward<Nth>(nth);
        }
      };
    };
  }

  template <std::size_t n, typename Tp>
  decltype(auto) get(Tp&& t) {
    return std::forward<Tp>(t).apply(detail::at_impl::get<n>{});
  }
}
}

#endif
