#ifndef ARCHIE_UTILS_ROUTER_H_INCLUDED
#define ARCHIE_UTILS_ROUTER_H_INCLUDED

#include <cstdint>
#include <boost/asio/buffer.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/range/iterator_range.hpp>
#include <archie/utils/requires.h>
#include <vector>

namespace archie {
namespace utils {

  namespace asio = boost::asio;
  namespace fusion = boost::fusion;

  template <typename Tp>
  using BuffType = typename Tp::buff_type;

  struct input {};
  struct output {};

  template <typename, typename>
  struct Router;

  template <typename Archive>
  using ArchiveType = typename Archive::Tag;

  template <typename Archive>
  struct isArchive
      : std::is_base_of<Router<BuffType<Archive>, ArchiveType<Archive>>,
                        Archive> {};

  template <typename Archive>
  struct isOutput : std::is_same<ArchiveType<Archive>, output> {};

  template <typename Archive>
  struct isInput : std::is_same<ArchiveType<Archive>, input> {};

  template <typename Archive>
  struct isOutputArchive : And<isArchive<Archive>, isOutput<Archive>> {};

  template <typename Archive>
  struct isInputArchive : And<isArchive<Archive>, isInput<Archive>> {};

  using Byte = std::uint8_t;

  struct RawBuffer : std::vector<Byte> {
    using BaseType = std::vector<Byte>;
    using BaseType::BaseType;
    template <typename ArchiveTag>
    struct BufferRange : boost::iterator_range<BaseType::pointer> {
      using Tag = ArchiveTag;
      using Range = boost::iterator_range<BaseType::pointer>;
      using Range::Range;
      using SelfType = BufferRange<Tag>;

      template <typename Tp,
                RequiresAll<std::is_integral<Tp>, isOutput<SelfType>>...>
      void push_back(Tp const& tp) {
        *((Tp*)begin()) = tp;
        advance_begin(sizeof(Tp));
      }

      template <typename Tp,
                RequiresAll<std::is_integral<Tp>, isInput<SelfType>>...>
      void pop_front(Tp& tp) {
        tp = *((Tp const*)begin());
        advance_begin(sizeof(Tp));
      }

      template <typename Tp,
                RequiresAll<std::is_integral<Tp>, isInput<SelfType>>...>
      Tp pop() {
        auto tp = Tp{};
        pop_front(tp);
        return tp;
      }
    };
    using InputRange = BufferRange<input>;
    using OutputRange = BufferRange<output>;

    InputRange input_range() { return InputRange(&front(), &back() + 1); }
    OutputRange output_range() { return OutputRange(&front(), &back() + 1); }
  };

  template <typename Archive, typename Tp>
  Archive& serialize(Archive& ar, Tp&& tp) {
    return ar & std::forward<Tp>(tp);
  }

  template <typename Tp, typename ArchiveTag>
  struct Router {
    using buff_type = Tp;
    using Tag = ArchiveTag;

    template <typename Iterator>
    Router(Iterator first, Iterator last)
        : Router(first, last, 0) {}

    template <typename Iterator>
    Router(Iterator first, Iterator last, std::uint32_t version)
        : Router(&(*first), std::distance(first, last), version) {}

    Router(void* first, std::uint32_t size, std::uint32_t version)
        : buffer_(first, size), size_(0), version_(version) {}

    template <typename Up>
    void advance(Up const&) {
      buffer_ = buffer_ + sizeof(Up);
      size_ += sizeof(Up);
    }

    buff_type& buffer() { return buffer_; }
    std::uint32_t size() const { return size_; }
    std::uint32_t version() const { return version_; }

  private:
    buff_type buffer_;
    std::uint32_t size_;
    std::uint32_t version_;
  };

  using Writer = Router<asio::mutable_buffer, output>;
  using Reader = Router<asio::const_buffer, input>;

  template <typename Tp, typename Archive,
            RequiresAll<isInputArchive<Archive>,
                        std::is_default_constructible<Tp>>...>
  Tp deserialize(Archive& ar) {
    auto tp = Tp{};
    serialize(ar, tp);
    return tp;
  }

  template <typename Archive, typename Tp,
            RequiresAll<isOutputArchive<Archive>, std::is_integral<Tp>>...>
  Archive& operator&(Archive& ar, Tp const& tp) {
    *asio::buffer_cast<Tp*>(ar.buffer()) = tp;
    ar.advance(tp);
    return ar;
  }

  template <typename Archive, typename Tp,
            RequiresAll<isInputArchive<Archive>, std::is_integral<Tp>>...>
  Archive& operator&(Archive& ar, Tp& tp) {
    tp = *asio::buffer_cast<Tp const*>(ar.buffer());
    ar.advance(tp);
    return ar;
  }

  template <typename Archive, typename Tp,
            RequiresAll<isArchive<Archive>, fusion::traits::is_sequence<Tp>>...>
  Archive& operator&(Archive& ar, Tp& tp) {
    auto do_serialize = [&ar](auto&& tp) { ar& tp; };
    fusion::for_each(tp, do_serialize);
    return ar;
  }

  template <typename Archive, typename Tp, typename Alloc,
            Requires<isOutputArchive<Archive>>...>
  Archive& operator&(Archive& ar, std::vector<Tp, Alloc> const& vec) {
    ar& vec.size();
    for (auto&& tp : vec) ar& tp;
    return ar;
  }

  template <typename Archive, typename Tp, typename Alloc,
            Requires<isInputArchive<Archive>>...>
  Archive& operator&(Archive& ar, std::vector<Tp, Alloc>& vec) {
    vec.resize(deserialize<typename std::vector<Tp, Alloc>::size_type>(ar));
    for (auto&& tp : vec) ar& tp;
    return ar;
  }

  template <typename Archive, Requires<isOutputArchive<Archive>>...>
  Archive& operator&(Archive& ar, RawBuffer const& buff) {
    ar& buff.size();
    auto it = asio::buffer_cast<RawBuffer::pointer>(ar.buffer());
    std::copy(std::begin(buff), std::end(buff), it);
    return ar;
  }

  template <typename Archive, Requires<isInputArchive<Archive>>...>
  Archive& operator&(Archive& ar, RawBuffer& buff) {
    buff.resize(deserialize<RawBuffer::size_type>(ar));
    auto it = asio::buffer_cast<RawBuffer::const_pointer>(ar.buffer());
    std::copy(it, it + buff.size(), std::begin(buff));
    return ar;
  }
}
}

#endif
