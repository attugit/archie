#pragma once

#include <utility>
#include <archie/utils/meta/functional.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/fused/tail.h>
#include <archie/utils/traits/is_fused_tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct compose_ {
      private:
        template <typename F, typename... Ts>
        constexpr decltype(auto) do_work_t(meta::true_t, F&& f,
                                           Ts&&... args) const {
          return do_work(meta::false_t{}, fused::get<0>(std::forward<F>(f)),
                         std::forward<Ts>(args)...);
        }
        template <typename Fs, typename... Ts>
        constexpr decltype(auto) do_work_t(meta::false_t, Fs&& fs,
                                           Ts&&... args) const {
          return fused::apply(fused::get<0>(std::forward<Fs>(fs)),
                              do_work(meta::true_t{},
                                      fused::tail(std::forward<Fs>(fs)),
                                      std::forward<Ts>(args)...));
        }
        template <typename F, typename... Ts>
        constexpr decltype(auto) do_work(meta::true_t, F&& f,
                                         Ts&&... args) const {
          return do_work_t(
              meta::boolean<(
                  fused::tuple_size<std::remove_reference_t<F>>::value == 1)>{},
              std::forward<F>(f), std::forward<Ts>(args)...);
        }
        template <typename F, typename... Ts>
        constexpr decltype(auto) do_work(meta::false_t, F&& f,
                                         Ts&&... args) const {
          return fused::apply(std::forward<F>(f), std::forward<Ts>(args)...);
        }

      public:
        template <typename F, typename... Ts>
        constexpr decltype(auto) operator()(F&& f, Ts&&... args) const {
          return do_work(traits::is_fused_tuple<F>{}, std::forward<F>(f),
                         std::forward<Ts>(args)...);
        }
      };
    }
    constexpr detail::compose_ compose{};
  }
}
}
