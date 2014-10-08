#include <iostream>
#include <archie/utils/router.h>
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
