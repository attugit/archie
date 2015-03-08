#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp, typename Up>
    struct append {
      using type = type_list<Tp, Up>;
    };

    template <typename Tp, typename... Us>
    struct append<type_list<Us...>, Tp> {
      using type = type_list<Us..., Tp>;
    };

    template <typename...>
    struct zip;

    template <typename... Xs, typename... Ys>
    struct zip<type_list<Xs...>, type_list<Ys...>> {
      static_assert(sizeof...(Xs) == sizeof...(Ys), "");
      using type = type_list<type_list<Xs, Ys>...>;
    };

    template <typename Tp, typename Up>
    using zip_t = typename zip<Tp, Up>::type;
  }
}
}

namespace meta = archie::utils::meta;
using meta::type_list;

#include <type_traits>

static_assert(std::is_same<meta::append<int, char>::type, type_list<int, char>>::value, "");

static_assert(std::is_same<meta::append<type_list<int, char>, unsigned>::type,
                           type_list<int, char, unsigned>>::value,
              "");

static_assert(
    std::is_same<meta::zip<type_list<int, char>, type_list<unsigned, unsigned char>>::type,
                 type_list<type_list<int, unsigned>, type_list<char, unsigned char>>>::value,
    "");

int main() { return 0; }
