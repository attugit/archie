#include <cstdint>
#include <boost/asio/buffer.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <archie/utils/requires.h>
#include <iostream>

namespace asio = boost::asio;
namespace fusion = boost::fusion;

namespace archie {
namespace utils {

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

#include <boost/fusion/include/define_struct.hpp>
#include <vector>

BOOST_FUSION_DEFINE_STRUCT((archie)(utils)(message), header,
                           (std::uint8_t, magic)(std::uint8_t, version)(
                               std::uint32_t, length)(std::uint32_t, type))

BOOST_FUSION_DEFINE_STRUCT((archie)(utils)(message), frame,
                           (archie::utils::message::header,
                            header)(std::vector<std::uint8_t>, data))

#include <gtest/gtest.h>

namespace {

struct message_test : ::testing::Test {};

TEST_F(message_test, nothing) {
  auto vec = std::vector<char>(128);

  using archie::utils::message::frame;
  auto inframe = frame();
  auto outframe = frame();

  using archie::utils::serialize;
  {
    auto& header = inframe.header;
    auto& data = inframe.data;

    header.magic = 3;
    header.version = 7;
    header.type = 0xdeadbeef;
    data = {7, 6, 5, 4, 3, 2, 1, 0};
    header.length = data.size();

    using archie::utils::Writer;
    auto writer = Writer(std::begin(vec), std::end(vec));
    serialize(writer, 0, inframe);
  }
  {
    using archie::utils::Reader;
    auto reader = Reader(std::begin(vec), std::end(vec));
    serialize(reader, 0, outframe);
  }

  ASSERT_EQ(3, inframe.header.magic);
  ASSERT_EQ(7, inframe.header.version);
  ASSERT_EQ(0xdeadbeef, inframe.header.type);
  ASSERT_EQ((std::vector<std::uint8_t>{7, 6, 5, 4, 3, 2, 1, 0}), inframe.data);
  ASSERT_EQ(inframe.data.size(), inframe.header.length);

  EXPECT_EQ(inframe.header.magic, outframe.header.magic);
  EXPECT_EQ(inframe.header.version, outframe.header.version);
  EXPECT_EQ(inframe.header.length, outframe.header.length);
  EXPECT_EQ(inframe.header.type, outframe.header.type);
  EXPECT_EQ(inframe.data, outframe.data);
}
}
