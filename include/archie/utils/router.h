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
  using BuffType = typename Tp::BuffType;

  struct input {};
  struct output {};

  template <typename>
  struct Router;

  template <typename Archive>
  using ArchiveType = typename Archive::Tag;

  template <typename Archive>
  struct isArchive : std::is_base_of<Router<BuffType<Archive>>, Archive> {};

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

  template <typename Tp>
  struct Router {
    using BuffType = Tp;
    using Tag = ArchiveType<BuffType>;

    template <typename Iterator>
    Router(Iterator first, Iterator last, std::uint32_t version)
        : buffer_(first, last), version_(version) {}

    template <typename Up>
    Router(Up&& up, std::uint32_t version)
        : buffer_(std::forward<Up>(up)), version_(version) {}

    template <typename Up>
    explicit Router(Up&& up)
        : Router(std::forward<Up>(up), 0) {}

    template <typename Up,
              RequiresAll<std::is_integral<Up>, isOutput<BuffType>>...>
    void push_back(Up const& up) {
      buffer().push_back(up);
    }

    template <typename Up,
              RequiresAll<std::is_integral<Up>, isInput<BuffType>>...>
    void pop_front(Up& up) {
      buffer().pop_front(up);
    }

    template <typename Up,
              RequiresAll<std::is_integral<Up>, isInput<BuffType>>...>
    Up pop() {
      return buffer().pop<Up>();
    }

    BuffType& buffer() { return buffer_; }
    std::uint32_t size() const { return buffer().size(); }
    std::uint32_t version() const { return version_; }

  private:
    BuffType buffer_;
    std::uint32_t version_;
  };

  using Writer = Router<RawBuffer::OutputRange>;
  using Reader = Router<RawBuffer::InputRange>;

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
    ar.push_back(tp);
    return ar;
  }

  template <typename Archive, typename Tp,
            RequiresAll<isInputArchive<Archive>, std::is_integral<Tp>>...>
  Archive& operator&(Archive& ar, Tp& tp) {
    ar.pop_front(tp);
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
    auto it = ar.buffer().begin();
    std::copy(std::begin(buff), std::end(buff), it);
    return ar;
  }

  template <typename Archive, Requires<isInputArchive<Archive>>...>
  Archive& operator&(Archive& ar, RawBuffer& buff) {
    buff.resize(deserialize<RawBuffer::size_type>(ar));
    auto it = ar.buffer().begin();
    std::copy(it, it + buff.size(), std::begin(buff));
    return ar;
  }
}
}

#endif
