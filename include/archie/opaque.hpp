#pragma once
#include <utility>

namespace archie {

template <typename...>
struct opaque;

template <typename>
struct tag {};

inline namespace feature {
  struct self {};

  struct extractable {};

  template <typename>
  struct equivalent {};

  template <typename>
  struct ordered {};

  template <typename>
  struct addable {};

  template <typename>
  struct assignable {};

  template <typename Tp, typename F>
  struct has : std::is_base_of<F, typename Tp::features>::type {};

  template <typename T, template <typename> class F, typename U, typename V, typename... W>
  struct has<T, F<opaque<U, V, W...>>>
      : std::integral_constant<bool,
                               std::is_base_of<F<opaque<U, V, W...>>, typename T::features>::value |
                                   std::is_base_of<F<tag<U>>, typename T::features>::value> {};

  template <typename Tp, typename F, typename R = void>
  using enabled = typename std::enable_if<has<Tp, F>::value, R>::type;

  template <typename...>
  struct get_f {
    using type = void;
  };

  template <typename F, typename H, typename... T>
  struct get_f<F, H, T...> {
    using type = typename std::conditional<std::is_base_of<F, H>::value,
                                           H,
                                           typename get_f<F, T...>::type>::type;
  };
}

template <typename>
struct is_opaque : std::false_type {};

template <typename... Ts>
struct is_opaque<opaque<Ts...>> : std::true_type {};

template <typename, typename>
struct is_opaque_of : std::false_type {};

template <typename T, typename U, typename V, typename... W>
struct is_opaque_of<opaque<T, U, W...>, V> : std::is_same<U, V> {};

template <typename Tp>
Tp const& extract(Tp const& x) {
  return x;
}

template <typename Tp>
Tp& extract(Tp& x) {
  return x;
}

template <typename Tag, typename Tp, typename... Fs>
struct opaque<Tag, Tp, Fs...> final {
  struct features : Fs... {};
  using value_type = Tp;

  template <typename... Us>
  explicit opaque(Us&&... args)
      : value_(std::forward<Us>(args)...) {}

  template <typename T, typename U, typename... V>
  friend enabled<opaque<T, U, V...>, extractable, U const&> extract(opaque<T, U, V...> const&);

  template <typename T, typename U, typename... V>
  friend enabled<opaque<T, U, V...>, extractable, U&> extract(opaque<T, U, V...>&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, equivalent<V>, bool> operator==(opaque<T, U, W...> const&,
                                                                     V const&);

  template <typename T, typename U, typename... W>
  friend enabled<opaque<T, U, W...>, equivalent<self>, bool> operator==(opaque<T, U, W...> const&,
                                                                        opaque<T, U, W...> const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, equivalent<V>, bool> operator!=(opaque<T, U, W...> const&,
                                                                     V const&);

  template <typename T, typename U, typename... W>
  friend enabled<opaque<T, U, W...>, equivalent<self>, bool> operator!=(opaque<T, U, W...> const&,
                                                                        opaque<T, U, W...> const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<V>, bool> operator<(opaque<T, U, W...> const&,
                                                                 V const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<V>, bool> operator>(opaque<T, U, W...> const&,
                                                                 V const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<V>, bool> operator<=(opaque<T, U, W...> const&,
                                                                  V const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<V>, bool> operator>=(opaque<T, U, W...> const&,
                                                                  V const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<V>, bool> operator<(V const&,
                                                                 opaque<T, U, W...> const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<V>, bool> operator>(V const&,
                                                                 opaque<T, U, W...> const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<V>, bool> operator<=(V const&,
                                                                  opaque<T, U, W...> const&);

  template <typename T, typename U, typename V, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<V>, bool> operator>=(V const&,
                                                                  opaque<T, U, W...> const&);

  template <typename T, typename U, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<self>, bool> operator<(opaque<T, U, W...> const&,
                                                                    opaque<T, U, W...> const&);

  template <typename T, typename U, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<self>, bool> operator>(opaque<T, U, W...> const&,
                                                                    opaque<T, U, W...> const&);

  template <typename T, typename U, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<self>, bool> operator<=(opaque<T, U, W...> const&,
                                                                     opaque<T, U, W...> const&);

  template <typename T, typename U, typename... W>
  friend enabled<opaque<T, U, W...>, ordered<self>, bool> operator>=(opaque<T, U, W...> const&,
                                                                     opaque<T, U, W...> const&);

  template <typename T, typename U, typename... W>
  friend enabled<opaque<T, U, W...>, addable<self>, opaque<T, U, W...>&> operator+=(
      opaque<T, U, W...>&,
      opaque<T, U, W...> const&);

  template <typename U>
  typename std::enable_if<feature::has<opaque, feature::assignable<U>>::value, opaque&>::type
  operator=(U const& u) {
    value_ = u;
    return *this;
  }

private:
  value_type value_;
};

template <typename T, typename U, typename... V>
enabled<opaque<T, U, V...>, extractable, U const&> extract(opaque<T, U, V...> const& op) {
  return op.value_;
}

template <typename T, typename U, typename... V>
enabled<opaque<T, U, V...>, extractable, U&> extract(opaque<T, U, V...>& op) {
  return op.value_;
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, equivalent<V>, bool> operator==(opaque<T, U, W...> const& lhs,
                                                            V const& rhs) {
  return lhs.value_ == extract(rhs);
}

template <typename T, typename U, typename... W>
enabled<opaque<T, U, W...>, equivalent<self>, bool> operator==(opaque<T, U, W...> const& lhs,
                                                               opaque<T, U, W...> const& rhs) {
  return lhs.value_ == rhs.value_;
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, equivalent<V>, bool> operator!=(opaque<T, U, W...> const& lhs,
                                                            V const& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename U, typename... W>
enabled<opaque<T, U, W...>, equivalent<self>, bool> operator!=(opaque<T, U, W...> const& lhs,
                                                               opaque<T, U, W...> const& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename U, typename... W>
enabled<opaque<T, U, W...>, ordered<self>, bool> operator<(opaque<T, U, W...> const& lhs,
                                                           opaque<T, U, W...> const& rhs) {
  return lhs.value_ < rhs.value_;
}

template <typename T, typename U, typename... W>
enabled<opaque<T, U, W...>, ordered<self>, bool> operator>(opaque<T, U, W...> const& lhs,
                                                           opaque<T, U, W...> const& rhs) {
  return rhs < lhs;
}

template <typename T, typename U, typename... W>
enabled<opaque<T, U, W...>, ordered<self>, bool> operator<=(opaque<T, U, W...> const& lhs,
                                                            opaque<T, U, W...> const& rhs) {
  return !(lhs > rhs);
}

template <typename T, typename U, typename... W>
enabled<opaque<T, U, W...>, ordered<self>, bool> operator>=(opaque<T, U, W...> const& lhs,
                                                            opaque<T, U, W...> const& rhs) {
  return !(lhs < rhs);
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, ordered<V>, bool> operator<(opaque<T, U, W...> const& lhs,
                                                        V const& rhs) {
  return lhs.value_ < extract(rhs);
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, ordered<V>, bool> operator>(opaque<T, U, W...> const& lhs,
                                                        V const& rhs) {
  return rhs < lhs;
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, ordered<V>, bool> operator<=(opaque<T, U, W...> const& lhs,
                                                         V const& rhs) {
  return !(lhs > rhs);
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, ordered<V>, bool> operator>=(opaque<T, U, W...> const& lhs,
                                                         V const& rhs) {
  return !(lhs < rhs);
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, ordered<V>, bool> operator<(V const& lhs,
                                                        opaque<T, U, W...> const& rhs) {
  return extract(lhs) < rhs.value_;
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, ordered<V>, bool> operator>(V const& lhs,
                                                        opaque<T, U, W...> const& rhs) {
  return rhs < lhs;
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, ordered<V>, bool> operator<=(V const& lhs,
                                                         opaque<T, U, W...> const& rhs) {
  return !(lhs > rhs);
}

template <typename T, typename U, typename V, typename... W>
enabled<opaque<T, U, W...>, ordered<V>, bool> operator>=(V const& lhs,
                                                         opaque<T, U, W...> const& rhs) {
  return !(lhs < rhs);
}

template <typename T, typename U, typename... W>
enabled<opaque<T, U, W...>, addable<self>, opaque<T, U, W...>&> operator+=(
    opaque<T, U, W...>& lhs,
    opaque<T, U, W...> const& rhs) {
  using F = typename get_f<addable<self>, W...>::type;
  lhs.value_ = F{}(lhs.value_, rhs.value_);
  return lhs;
}
}
