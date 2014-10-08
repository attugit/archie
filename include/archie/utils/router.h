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
  struct Serialize {
    template <typename Archive>
    static Archive& exec(Archive& ar, std::uint32_t,
                         ParamType<Archive, Tp> tp) {
      ar& tp;
      return ar;
    }
  };

  struct input {};
  struct output {};

  template <typename Archive, typename Tp>
  Archive& serialize(Archive& ar, std::uint32_t version, Tp&& tp) {
    using type = std::remove_reference_t<Tp>;
    return Serialize<type>::exec(ar, version, std::forward<Tp>(tp));
  }

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
    using BaseType::buffer;

    template <typename Tp>
    using ParamType = Tp const&;
  };

  template <typename Tp, Requires<std::is_integral<Tp>>...>
  Writer& operator&(Writer& writer, Tp const& tp) {
    *asio::buffer_cast<Tp*>(writer.buffer()) = tp;
    writer.advance<Tp>();
    return writer;
  }

  template <typename Tp, Requires<fusion::traits::is_sequence<Tp>>...>
  Writer& operator&(Writer& writer, Tp const& tp) {
    auto do_serialize = [&writer](auto&& tp) { writer& tp; };
    fusion::for_each(tp, do_serialize);
    return writer;
  }

  template <typename Tp, typename Alloc>
  Writer& operator&(Writer& writer, std::vector<Tp, Alloc> const& vec) {
    writer& vec.size();
    for (auto&& tp : vec) writer& tp;
    return writer;
  }

  struct Reader : Router<asio::const_buffer> {
    using BaseType = Router<asio::const_buffer>;
    using Tag = input;
    using BaseType::BaseType;
    using BaseType::advance;
    using BaseType::buffer;

    template <typename Tp>
    using ParamType = Tp&;
  };

  template <typename Tp, Requires<std::is_integral<Tp>>...>
  Reader& operator&(Reader& reader, Tp& tp) {
    tp = *asio::buffer_cast<Tp const*>(reader.buffer());
    reader.advance<Tp>();
    return reader;
  }

  template <typename Tp, Requires<fusion::traits::is_sequence<Tp>>...>
  Reader& operator&(Reader& reader, Tp& tp) {
    auto do_serialize = [&reader](auto&& tp) { reader& tp; };
    fusion::for_each(tp, do_serialize);
    return reader;
  }

  template <typename Tp, typename Alloc>
  Reader& operator&(Reader& reader, std::vector<Tp, Alloc>& vec) {
    using size_type = decltype(std::declval<std::vector<Tp, Alloc>>().size());
    auto size = size_type(0);
    reader& size;
    vec.resize(size);
    for (auto&& tp : vec) reader& tp;
    return reader;
  }
}
}

#endif
