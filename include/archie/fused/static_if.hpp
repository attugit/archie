#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::fused
{
  namespace detail
  {
    struct static_if_ {
      template <typename Cond>
      decltype(auto) operator()(Cond const&) const
      {
        return select(std::integral_constant<bool, std::decay_t<Cond>::value>{});
      }

    private:
      template <typename F>
      struct result_ : F {
        template <typename T>
        explicit result_(T&& t) noexcept : F(std::forward<T>(t))
        {
        }

        using F::operator();

        template <typename T>
        auto& then(T&&) noexcept
        {
          return *this;
        }
        template <typename T>
        auto& then(T&&) const noexcept
        {
          return *this;
        }
        template <typename T>
        auto& else_(T&&) noexcept
        {
          return *this;
        }
        template <typename T>
        auto& else_(T&&) const noexcept
        {
          return *this;
        }
        template <typename T>
        auto& else_if(T&&) noexcept
        {
          return *this;
        }
        template <typename T>
        auto& else_if(T&&) const noexcept
        {
          return *this;
        }
      };
      struct if_true_ {
        template <typename F>
        decltype(auto) then(F&& f) const noexcept
        {
          return result_<F>(std::forward<F>(f));
        }
        template <typename F>
        auto const& else_(F&&) const noexcept
        {
          return *this;
        }
        template <typename F>
        auto const& else_if(F&&) const noexcept
        {
          return *this;
        }
      };
      struct if_false_ {
        template <typename F>
        auto const& then(F&&) const noexcept
        {
          return *this;
        }
        template <typename F>
        decltype(auto) else_(F&& f) const noexcept
        {
          return result_<F>(std::forward<F>(f));
        }
        template <typename Cond>
        decltype(auto) else_if(Cond const& c) const noexcept
        {
          return static_if_{}(c);
        }
        template <typename T>
        auto operator()(T&&) const noexcept
        {
        }
      };
      decltype(auto) select(std::true_type) const noexcept { return if_true_{}; }
      decltype(auto) select(std::false_type) const noexcept { return if_false_{}; }
    };
  }
  static constexpr auto const& static_if = meta::instance<detail::static_if_>();
}
