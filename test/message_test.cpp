#include <cstdint>
#include <boost/asio/buffer.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <archie/utils/requires.h>
#include <iostream>

namespace asio = boost::asio;
namespace fusion = boost::fusion;

namespace archie {
namespace router {

  template <typename Tp>
  struct Router {
    using buff_type = Tp;

    template <typename Iterator>
    Router(Iterator first, Iterator last)
        : buffer_(&(*first), std::distance(first, last)), length(0) {}

    template <typename Up>
    void advance(std::uint32_t s = sizeof(Up)) {
      buffer_ = buffer_ + s;
      length += s;
    }

    std::uint32_t size() const { return length; }

  protected:
    buff_type& buffer() { return buffer_; }

  private:
    buff_type buffer_;
    std::uint32_t length;
  };

  struct Writer : Router<asio::mutable_buffer> {
    using BaseType = Router<asio::mutable_buffer>;

    using BaseType::BaseType;

    template <typename Tp>
    Writer& operator&(Tp const& tp) {
      emplace_back(tp);
      return *this;
    }

  private:
    template <typename Cond>
    using requires = archie::utils::Requires<Cond>;

    template <typename T, requires<std::is_integral<T>>...>
    void emplace_back(T const& t) {
      *asio::buffer_cast<T*>(BaseType::buffer()) = t;
      BaseType::advance<T>();
    }

    template <typename T, requires<fusion::traits::is_sequence<T>>...>
    void emplace_back(T const& t) {
      auto do_write = [this](auto&& t) { this->emplace_back(t); };
      boost::fusion::for_each(t, do_write);
    }
    template <typename Tp>
    void emplace_back(std::vector<Tp> const& v) {
      emplace_back(v.size());
      for (auto&& t : v) emplace_back(t);
    }
  };

  struct Reader : Router<asio::const_buffer> {
    using BaseType = Router<asio::const_buffer>;

    using BaseType::BaseType;

    template <typename Tp>
    Reader& operator&(Tp& tp) {
      get(tp);
      return *this;
    }

  private:
    template <typename Cond>
    using requires = archie::utils::Requires<Cond>;

    template <typename T, requires<std::is_integral<T>>...>
    void get(T& t) {
      t = *asio::buffer_cast<T const*>(BaseType::buffer());
      BaseType::advance<T>();
    }
    template <typename T, requires<fusion::traits::is_sequence<T>>...>
    void get(T& t) {
      auto do_read = [this](auto&& t) { this->get(t); };
      boost::fusion::for_each(t, do_read);
    }
    template <typename Tp>
    void get(std::vector<Tp>& v) {
      auto size = v.size();
      get(size);
      v.resize(size);
      for (auto&& t : v) get(t);
    }
  };

  template <typename Container, typename Tp>
  std::size_t write(Container& c, Tp const& tp) {
    auto writer = Writer(std::begin(c), std::end(c));
    writer(tp);
    return writer.size();
  }
}
}

#include <boost/fusion/include/define_struct.hpp>
#include <vector>

BOOST_FUSION_DEFINE_STRUCT((archie)(router)(message), header,
                           (std::uint8_t, magic)(std::uint8_t, version)(
                               std::uint32_t, length)(std::uint32_t, type))

BOOST_FUSION_DEFINE_STRUCT((archie)(router)(message), frame,
                           (archie::router::message::header,
                            header)(std::vector<std::uint8_t>, data))

#include <gtest/gtest.h>

namespace {

struct message_test : ::testing::Test {};

TEST_F(message_test, nothing) {
  std::vector<char> vec(128);
  auto inframe = archie::router::message::frame();
  auto outframe = archie::router::message::frame();
  {
    auto& header = inframe.header;
    auto& data = inframe.data;

    header.magic = 3;
    header.version = 7;
    header.type = 0xdeadbeef;
    data = {7, 6, 5, 4, 3, 2, 1, 0};

    header.length = data.size();

    auto writer = archie::router::Writer(std::begin(vec), std::end(vec));
    writer& inframe;
  }
  {
    auto reader = archie::router::Reader(std::begin(vec), std::end(vec));
    reader& outframe;
  }

  EXPECT_EQ(inframe.header.magic, outframe.header.magic);
  EXPECT_EQ(inframe.header.version, outframe.header.version);
  EXPECT_EQ(inframe.header.length, outframe.header.length);
  EXPECT_EQ(inframe.header.type, outframe.header.type);
  EXPECT_EQ(inframe.data, outframe.data);
}
}
