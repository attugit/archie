#ifndef ARCHIE_UTILS_VARIADIC_H_INCLUDED
#define ARCHIE_UTILS_VARIADIC_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {

  template <std::size_t N>
  using Number = std::integral_constant<std::size_t, N>;

  template <typename Tp>
  using Alias = Tp;

  namespace detail {
    template <typename Tp, typename...>
    struct VariadicWrapper {
      using type = Tp;
    };
  }

  template <typename... Ts>
  using Voider = typename detail::VariadicWrapper<void, Ts...>::type;

  template <template <typename> class, typename, typename = void>
  struct HasMember : std::false_type {};

  template <template <typename> class Member, typename Tp>
  struct HasMember<Member, Tp, Voider<Member<Tp>>> : std::true_type {};

  template <typename Tp>
  using ValueType = typename Tp::value_type;

  template <typename Tp>
  using VariadicType = typename std::remove_reference_t<Tp>::variadic;

  template <typename Tp>
  using HasValueType = HasMember<ValueType, Tp>;

  template <typename Tp>
  using HasVariadicType = HasMember<VariadicType, Tp>;

  template <typename Func, typename... Args>
  constexpr void for_each(Func&& func, Args&&... args) {
    (void)Alias<int[]>{(func(std::forward<Args>(args)), 1)...};
  }

  template <typename Func, typename... Ts>
  constexpr decltype(auto) apply(Func&& func, Ts&&... ts) noexcept(
      noexcept(std::declval<Func>()(std::declval<Ts>()...))) {
    return std::forward<Func>(func)(std::forward<Ts>(ts)...);
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
      return Number<Idx>{};
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

  template <std::size_t n, typename TupleType>
  constexpr decltype(auto) get(TupleType&& t) noexcept {
    return std::forward<TupleType>(t).apply(detail::at_impl::get<n>{});
  }

  template <typename Tp, typename TupleType>
  constexpr decltype(auto) get(TupleType&& tuple) noexcept {
    using index = Number<VariadicType<TupleType>::template index_of<Tp>()>;
    return get<index::value>(std::forward<TupleType>(tuple));
  }
}
}

#endif
