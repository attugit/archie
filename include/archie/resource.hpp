#pragma once
#include <utility>
#include <experimental/optional>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {

static constexpr auto const& in_place = std::experimental::in_place;

struct null_resource_t {};
static constexpr auto const& null_resource = meta::instance<null_resource_t>();

template <typename T, typename Deleter>
struct resource {
  using deleter_type = std::experimental::optional<Deleter>;
  using value_type = T;
  using pointer = T*;
  using const_pointer = T const*;
  using reference = T&;
  using const_reference = T const&;

  template <typename U,
            typename D,
            typename = typename std::enable_if<!std::is_same<U, decltype(in_place)>::value>::type>
  explicit resource(U&& u, D&& del)
      : r(std::forward<U>(u)), d(std::forward<D>(del)) {}

  template <typename... Args>
  explicit resource(decltype(in_place), Args&&... args)
      : r(std::forward<Args>(args)...), d(in_place) {}

  resource(resource const&) = delete;
  resource& operator=(resource const&) = delete;
  resource(resource&&) = default;
  resource& operator=(resource&&) = default;
  ~resource() {
    if (d) (*d)(r);
  }

  reference operator*() { return r; }
  const_reference operator*() const { return r; }
  pointer operator->() { return &r; }
  const_pointer operator->() const { return &r; }

  template <typename... Args>
  void engage(Args&&... args) {
    d = deleter_type(in_place, std::forward<Args>(args)...);
  }

  resource& operator=(null_resource_t const&) {
    release();
    return *this;
  }

  value_type release() {
    d = std::experimental::nullopt;
    return std::move(r);
  }

private:
  value_type r;
  deleter_type d;
};

namespace detail {
  struct make_resource_ {
    template <typename T, typename D>
    auto operator()(T&& t, D&& d) const {
      return resource<std::remove_reference_t<T>, std::remove_reference_t<D>>(std::forward<T>(t),
                                                                              std::forward<D>(d));
    }
  };
}

static constexpr auto const& make_resource = meta::instance<detail::make_resource_>();
}
