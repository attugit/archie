#include <iostream>
#include <archie/utils/router.h>
#include <boost/fusion/include/define_struct.hpp>
#include <vector>

BOOST_FUSION_DEFINE_STRUCT((archie)(utils)(message), header,
                           (std::uint8_t, magic)(std::uint8_t, version)(
                               std::uint32_t, length)(std::uint32_t, type))

BOOST_FUSION_DEFINE_STRUCT((archie)(utils)(message), frame,
                           (archie::utils::message::header,
                            header)(archie::utils::RawBuffer, data))

#include <test/assert.h>

void testing() {
  using archie::utils::RawBuffer;
  auto buff = RawBuffer(128);

  using archie::utils::message::frame;
  auto inframe = frame();

  using archie::utils::serialize;
  {
    auto& header = inframe.header;
    auto& data = inframe.data;

    header.magic = 3;
    header.version = 7;
    header.type = 0xdeadbeef;
    header.length = 16;
    data.resize(header.length);
    auto output = data.output_range();
    output.push_back(4);
    output.push_back(129U);

    using archie::utils::Writer;
    auto writer = Writer(buff.output_range());
    serialize(writer, inframe);
  }

  ASSERT_EQ(3, inframe.header.magic);
  ASSERT_EQ(7, inframe.header.version);
  ASSERT_EQ(0xdeadbeef, inframe.header.type);
  ASSERT_EQ(inframe.data.size(), inframe.header.length);

  {
    using archie::utils::Reader;
    using archie::utils::deserialize;
    auto reader = Reader(buff.input_range());
    auto outframe = deserialize<frame>(reader);

    EXPECT_EQ(inframe.header.magic, outframe.header.magic);
    EXPECT_EQ(inframe.header.version, outframe.header.version);
    EXPECT_EQ(inframe.header.length, outframe.header.length);
    EXPECT_EQ(inframe.header.type, outframe.header.type);

    auto input = outframe.data.input_range();
    EXPECT_EQ(4, input.pop<int>());
    EXPECT_EQ(129U, input.pop<unsigned>());
  }
}

int main() {
  testing();
  return 0;
}
