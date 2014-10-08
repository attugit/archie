#ifndef ARCHIE_UTILS_ROUTER_H_INCLUDED
#define ARCHIE_UTILS_ROUTER_H_INCLUDED

#include <cstdint>
#include <boost/asio/buffer.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <archie/utils/requires.h>
#include <vector>

namespace archie {
namespace utils {

  namespace asio = boost::asio;
  namespace fusion = boost::fusion;

  template <typename Archive, typename Tp>
  using ParamType = typename Archive::template ParamType<Tp>;

  template <typename Tp, typename Enable = detail::enabler>
  struct Serialize;

  struct input {};
  struct output {};

  template <typename Archive>
  using ArchiveType = typename Archive::Tag;

  template <typename Archive, typename Enable = detail::enabler>
  struct isInputArchive : std::false_type {};

  template <typename Archive>
  struct isInputArchive<Archive,
                        Requires<std::is_same<input, ArchiveType<Archive>>>>
      : std::true_type {};

  template <typename Archive, typename Tp>
  Archive& serialize(Archive& ar, std::uint32_t version, Tp&& tp) {
    using type = std::remove_reference_t<Tp>;
    return Serialize<type>::exec(ar, version, std::forward<Tp>(tp));
  }

  template <typename Tp>
  struct Serialize<Tp, Requires<std::is_integral<Tp>>> {
    template <typename Archive>
    static Archive& exec(Archive& ar, std::uint32_t,
                         ParamType<Archive, Tp> tp) {
      ar& tp;
      return ar;
    }
  };

  template <typename Tp>
  struct Serialize<Tp, Requires<fusion::traits::is_sequence<Tp>>> {
    template <typename Archive>
    static Archive& exec(Archive& ar, std::uint32_t version,
                         ParamType<Archive, Tp> tp) {
      auto do_serialize = [&ar, version](auto&& tp) {
        using type = std::remove_cv_t<std::remove_reference_t<decltype(tp)>>;
        Serialize<type>::exec(ar, version, tp);
      };
      fusion::for_each(tp, do_serialize);
      return ar;
    }
  };

  template <typename Tp, typename Alloc>
  struct Serialize<std::vector<Tp, Alloc>> {
    template <typename Archive, Requires<Not<isInputArchive<Archive>>>...>
    static Archive& exec(Archive& ar, std::uint32_t version,
                         ParamType<Archive, std::vector<Tp, Alloc>> vec) {
      serialize(ar, version, vec.size());
      for (auto&& tp : vec) serialize(ar, version, tp);
      return ar;
    }
    template <typename Archive,
              RequiresAll<isInputArchive<Archive>,
                          std::is_default_constructible<Tp>>...>
    static Archive& exec(Archive& ar, std::uint32_t version,
                         ParamType<Archive, std::vector<Tp, Alloc>> vec) {
      using vector_type = std::remove_reference_t<decltype(vec)>;
      using size_type = decltype(std::declval<vector_type>().size());
      auto size = size_type{0};
      serialize(ar, version, size);
      vec.resize(size);
      for (auto&& tp : vec) serialize(ar, version, tp);
      return ar;
    }
  };

  template <typename Tp>
  struct Router {
    using buff_type = Tp;

    template <typename Iterator>
    Router(Iterator first, Iterator last)
        : buffer_(&(*first), std::distance(first, last)), length(0) {}

    std::uint32_t size() const { return length; }

    template <typename Up>
    void advance(std::uint32_t s = sizeof(Up)) {
      buffer_ = buffer_ + s;
      length += s;
    }

  protected:
    buff_type& buffer() { return buffer_; }

  private:
    buff_type buffer_;
    std::uint32_t length;
  };

  struct Writer : Router<asio::mutable_buffer> {
    using BaseType = Router<asio::mutable_buffer>;
    using Tag = output;
    using BaseType::BaseType;
    using BaseType::advance;

    template <typename Tp>
    using ParamType = Tp const&;

    template <typename Tp>
    Writer& operator&(ParamType<Tp> tp) {
      emplace_back(tp);
      return *this;
    }

  private:
    template <typename Cond>
    using requires = archie::utils::Requires<Cond>;

    template <typename Tp, requires<std::is_integral<Tp>>...>
    void emplace_back(Tp const& tp) {
      *asio::buffer_cast<Tp*>(BaseType::buffer()) = tp;
      advance<Tp>();
    }
  };

  struct Reader : Router<asio::const_buffer> {
    using BaseType = Router<asio::const_buffer>;
    using Tag = input;

    using BaseType::BaseType;
    using BaseType::advance;

    template <typename Tp>
    using ParamType = Tp&;

    template <typename Tp>
    Reader& operator&(ParamType<Tp> tp) {
      get(tp);
      return *this;
    }

  private:
    template <typename Cond>
    using requires = archie::utils::Requires<Cond>;

    template <typename Tp, requires<std::is_integral<Tp>>...>
    void get(Tp& tp) {
      tp = *asio::buffer_cast<Tp const*>(BaseType::buffer());
      advance<Tp>();
    }
  };
}
}

#endif
