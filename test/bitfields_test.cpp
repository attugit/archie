#include <archie/utils/bitfields.h>
#include <gtest/gtest.h>
namespace {

namespace au = archie::utils;

struct bitfields_test : public ::testing::Test {
  au::IndexType i0 = au::IndexType(0);
  au::IndexType i1 = au::IndexType(1);
  au::IndexType i2 = au::IndexType(2);
  using pack_t = au::Pack<1, 2, 3>;
  using field_t = decltype(std::declval<pack_t>().make_field<1>());
  pack_t pack;

  void SetUp() { pack.set(0b010101); }
};

TEST_F(bitfields_test, storage) {
  static_assert(sizeof(au::Storage<0>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(au::Storage<8>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(au::Storage<9>) <= sizeof(std::uint16_t), "");
  static_assert(sizeof(au::Storage<16>) <= sizeof(std::uint16_t), "");
  static_assert(sizeof(au::Storage<17>) <= sizeof(std::uint32_t), "");
  static_assert(sizeof(au::Storage<32>) <= sizeof(std::uint32_t), "");
  static_assert(sizeof(au::Storage<33>) <= sizeof(std::uint64_t), "");
  static_assert(sizeof(au::Storage<64>) <= sizeof(std::uint64_t), "");
}

TEST_F(bitfields_test, pack) {
  static_assert(sizeof(au::Pack<0>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(au::Pack<3, 5>) <= sizeof(std::uint8_t), "");
  static_assert(sizeof(au::Pack<4, 5>) <= sizeof(std::uint16_t), "");
  static_assert(sizeof(au::Pack<3, 5, 8>) <= sizeof(std::uint16_t), "");
  static_assert(sizeof(au::Pack<10, 5, 2>) <= sizeof(std::uint32_t), "");
  static_assert(sizeof(au::Pack<15, 15, 2>) <= sizeof(std::uint32_t), "");
  static_assert(sizeof(au::Pack<10, 10, 13>) <= sizeof(std::uint64_t), "");
  static_assert(sizeof(au::Pack<30, 20, 10, 4>) <= sizeof(std::uint64_t), "");
}

TEST_F(bitfields_test, pack_element_offset) {
  constexpr pack_t pack;
  static_assert(au::offset_of<0>(pack) == 0, "");
  static_assert(au::offset_of<1>(pack) == 1, "");
  static_assert(au::offset_of<2>(pack) == 3, "");
}

TEST_F(bitfields_test, pack_element_size) {
  constexpr pack_t pack;
  static_assert(au::size_of<0>(pack) == 1, "");
  static_assert(au::size_of<1>(pack) == 2, "");
  static_assert(au::size_of<2>(pack) == 3, "");
}

TEST_F(bitfields_test, pack_size) {
  constexpr pack_t pack;
  static_assert(pack.size() == 6, "");
}

TEST_F(bitfields_test, pack_length) {
  constexpr pack_t pack;
  static_assert(pack.length() == 3, "");
}

TEST_F(bitfields_test, pack_mask_of) {
  static_assert(pack_t::mask_of<0>() == 0b000001, "");
  static_assert(pack_t::mask_of<1>() == 0b000110, "");
  static_assert(pack_t::mask_of<2>() == 0b111000, "");
}

TEST_F(bitfields_test, pack_make_single_field) {
  auto field = pack.make_field<0>();
  EXPECT_EQ(0u, field.index());
  EXPECT_EQ(0u, field.offset());
  EXPECT_EQ(1u, field.size());
  EXPECT_EQ(0b000001, field.mask());
}

TEST_F(bitfields_test, pack_make_many_field) {
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_EQ(0u, f0.index());
  EXPECT_EQ(1u, f1.index());
  EXPECT_EQ(2u, f2.index());

  EXPECT_EQ(0u, f0.offset());
  EXPECT_EQ(1u, f1.offset());
  EXPECT_EQ(3u, f2.offset());

  EXPECT_EQ(1u, f0.size());
  EXPECT_EQ(2u, f1.size());
  EXPECT_EQ(3u, f2.size());

  EXPECT_EQ(0b000001, f0.mask());
  EXPECT_EQ(0b000110, f1.mask());
  EXPECT_EQ(0b111000, f2.mask());
}

TEST_F(bitfields_test, pack_field_value) {
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_EQ(0b1, f0.value());
  EXPECT_EQ(0b10, f1.value());
  EXPECT_EQ(0b10, f2.value());
}

TEST_F(bitfields_test, pack_field_test) {
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_TRUE(f0.test(i0));
  EXPECT_FALSE(f1.test(i0));
  EXPECT_TRUE(f1.test(i1));
  EXPECT_FALSE(f2.test(i0));
  EXPECT_TRUE(f2.test(i1));
  EXPECT_FALSE(f2.test(i2));
}

TEST_F(bitfields_test, pack_field_set) {
  auto field = pack.make_field<1>();
  ASSERT_FALSE(field.test(i0));
  ASSERT_TRUE(field.test(i1));
  field.set(i1);
  EXPECT_FALSE(field.test(i0));
  EXPECT_TRUE(field.test(i1));
  field.set(i0);
  EXPECT_TRUE(field.test(i0));
  EXPECT_TRUE(field.test(i1));
  EXPECT_EQ(0b010111, pack.get());
}

TEST_F(bitfields_test, pack_field_reset) {
  auto field = pack.make_field<1>();
  ASSERT_FALSE(field.test(i0));
  ASSERT_TRUE(field.test(i1));
  field.reset(i0);
  EXPECT_FALSE(field.test(i0));
  EXPECT_TRUE(field.test(i1));
  field.reset(i1);
  EXPECT_FALSE(field.test(i0));
  EXPECT_FALSE(field.test(i1));
  EXPECT_EQ(0b010001, pack.get());
}

TEST_F(bitfields_test, pack_field_all) {
  pack.set(0b010110);
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_FALSE(f0.all());
  EXPECT_TRUE(f1.all());
  EXPECT_FALSE(f2.all());
}

TEST_F(bitfields_test, pack_field_any) {
  pack.set(0b010110);
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_FALSE(f0.any());
  EXPECT_TRUE(f1.any());
  EXPECT_TRUE(f2.any());
}

TEST_F(bitfields_test, pack_field_none) {
  pack.set(0b010110);
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_TRUE(f0.none());
  EXPECT_FALSE(f1.none());
  EXPECT_FALSE(f2.none());
}

TEST_F(bitfields_test, pack_field_chain) {
  EXPECT_TRUE((pack.make_field<0>().all()));
  EXPECT_TRUE((pack.make_field<1>().test(i1)));
  EXPECT_TRUE((pack.make_field<2>().reset(i1).none()));
}

TEST_F(bitfields_test, pack_field_eq) {
  au::Pack<1, 2, 1, 2> pack;
  pack.set(0b010101);
  auto field = pack.make_field<0>();
  EXPECT_TRUE(field == pack.make_field<0>());
  EXPECT_FALSE(field == pack.make_field<2>());
  pack.make_field<2>().set(i0);
  EXPECT_TRUE(field == pack.make_field<2>());
}

TEST_F(bitfields_test, pack_field_less) {
  au::Pack<1, 2, 1, 2> pack;
  pack.set(0b010101);
  auto field = pack.make_field<1>();
  EXPECT_FALSE(field < pack.make_field<3>());
  pack.make_field<3>().set(i1);
  EXPECT_TRUE(field < pack.make_field<3>());
}

TEST_F(bitfields_test, pack_set) {
  pack.set<field_t>(i0);
  EXPECT_EQ(0b010111, pack.get());
}

TEST_F(bitfields_test, pack_reset) {
  pack.reset<field_t>(i1);
  EXPECT_EQ(0b010001, pack.get());
}
}
