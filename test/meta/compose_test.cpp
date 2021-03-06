#include <archie/meta/compose.hpp>
#include <archie/number.hpp>
#include <memory>

namespace
{
  namespace au = archie;

  using _0 = au::meta::number<0>;
  using _1 = au::meta::number<1>;

  template <typename Tp>
  struct uptr_ {
    using type = std::unique_ptr<Tp>;
  };

  template <typename... Xs>
  struct size {
    using type = au::meta::number<sizeof...(Xs)>;
  };

  template <typename... Xs>
  using foo = au::meta::compose<uptr_>::apply<Xs...>;

  template <typename... Xs>
  using goo = au::meta::compose<uptr_, size>::apply<Xs...>;

  static_assert(std::is_same<std::unique_ptr<_0>, foo<_0>::type>::value, "");
  static_assert(std::is_same<std::unique_ptr<au::meta::number<2>>, goo<_0, _1>::type>::value, "");
}
