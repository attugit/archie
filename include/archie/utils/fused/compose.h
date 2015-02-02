#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/fused/tail.h>
#include <archie/utils/traits/is_fused_tuple.h>
#include <archie/utils/meta/logic.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct compose_ {
      private:
        template <typename F, typename... Ts>
        constexpr decltype(auto) do_work_t(meta::true_t, F&& f,
                                           Ts&&... args) const {
          return do_work(meta::false_t{}, fused::get<0>(f),
                         std::forward<Ts>(args)...);
        }
        template <typename Fs, typename... Ts>
        constexpr decltype(auto) do_work_t(meta::false_t, Fs&& fs,
                                           Ts&&... args) const {
          return fused::apply(fused::get<0>(fs),
                              do_work(meta::true_t{}, fused::tail(fs),
                                      std::forward<Ts>(args)...));
        }
        template <typename F, typename... Ts>
        constexpr decltype(auto) do_work(meta::true_t, F&& f,
                                         Ts&&... args) const {
          using is_single =
              meta::boolean<fused::tuple_size<std::decay_t<F>>::value == 1>;
          return do_work_t(meta::if_t<is_single, meta::true_t, meta::false_t>{},
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
