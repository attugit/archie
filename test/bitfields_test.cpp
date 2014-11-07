#include <archie/utils/bitfields.h>
#include <gtest/gtest.h>
namespace {

namespace au = archie::utils;

struct bitfields_test : public ::testing::Test {};

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
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(au::offset_of<0>(pack) == 0, "");
  static_assert(au::offset_of<1>(pack) == 1, "");
  static_assert(au::offset_of<2>(pack) == 3, "");
}

TEST_F(bitfields_test, pack_element_size) {
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(au::size_of<0>(pack) == 1, "");
  static_assert(au::size_of<1>(pack) == 2, "");
  static_assert(au::size_of<2>(pack) == 3, "");
}

TEST_F(bitfields_test, pack_size) {
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(pack.size() == 6, "");
}

TEST_F(bitfields_test, pack_length) {
  constexpr au::Pack<1, 2, 3> pack;
  static_assert(pack.length() == 3, "");
}

TEST_F(bitfields_test, pack_mask_of) {
  using pack_t = au::Pack<1, 2, 3>;
  static_assert(pack_t::mask_of<0>() == 0b000001, "");
  static_assert(pack_t::mask_of<1>() == 0b000110, "");
  static_assert(pack_t::mask_of<2>() == 0b111000, "");
}

TEST_F(bitfields_test, pack_make_single_field) {
  au::Pack<1, 2, 3> pack;
  auto field = pack.make_field<0>();
  EXPECT_EQ(0u, field.index());
  EXPECT_EQ(0u, field.offset());
  EXPECT_EQ(1u, field.size());
  EXPECT_EQ(0b000001, field.mask());
}

TEST_F(bitfields_test, pack_make_many_field) {
  au::Pack<1, 2, 3> pack;
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
  au::Pack<1, 2, 3> pack;
  pack.set(0b010101);
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_EQ(0b1, f0.value());
  EXPECT_EQ(0b10, f1.value());
  EXPECT_EQ(0b10, f2.value());
}

TEST_F(bitfields_test, pack_field_test) {
  au::Pack<1, 2, 3> pack;
  pack.set(0b010101);
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_TRUE(f0.test(0));
  EXPECT_FALSE(f1.test(0));
  EXPECT_TRUE(f1.test(1));
  EXPECT_FALSE(f2.test(0));
  EXPECT_TRUE(f2.test(1));
  EXPECT_FALSE(f2.test(2));
}

TEST_F(bitfields_test, pack_field_set) {
  au::Pack<1, 2, 3> pack;
  pack.set(0b010101);
  auto field = pack.make_field<1>();
  ASSERT_FALSE(field.test(0));
  ASSERT_TRUE(field.test(1));
  field.set(1);
  EXPECT_FALSE(field.test(0));
  EXPECT_TRUE(field.test(1));
  field.set(0);
  EXPECT_TRUE(field.test(0));
  EXPECT_TRUE(field.test(1));
  EXPECT_EQ(0b010111, pack.get());
}

TEST_F(bitfields_test, pack_field_reset) {
  au::Pack<1, 2, 3> pack;
  pack.set(0b010101);
  auto field = pack.make_field<1>();
  ASSERT_FALSE(field.test(0));
  ASSERT_TRUE(field.test(1));
  field.reset(0);
  EXPECT_FALSE(field.test(0));
  EXPECT_TRUE(field.test(1));
  field.reset(1);
  EXPECT_FALSE(field.test(0));
  EXPECT_FALSE(field.test(1));
  EXPECT_EQ(0b010001, pack.get());
}

TEST_F(bitfields_test, pack_field_all) {
  au::Pack<1, 2, 3> pack;
  pack.set(0b010110);
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_FALSE(f0.all());
  EXPECT_TRUE(f1.all());
  EXPECT_FALSE(f2.all());
}

TEST_F(bitfields_test, pack_field_any) {
  au::Pack<1, 2, 3> pack;
  pack.set(0b010110);
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_FALSE(f0.any());
  EXPECT_TRUE(f1.any());
  EXPECT_TRUE(f2.any());
}

TEST_F(bitfields_test, pack_field_none) {
  au::Pack<1, 2, 3> pack;
  pack.set(0b010110);
  auto f0 = pack.make_field<0>();
  auto f1 = pack.make_field<1>();
  auto f2 = pack.make_field<2>();
  EXPECT_TRUE(f0.none());
  EXPECT_FALSE(f1.none());
  EXPECT_FALSE(f2.none());
}

TEST_F(bitfields_test, pack_field_chain) {
  au::Pack<1, 2, 3> pack;
  pack.set(0b010101);
  EXPECT_TRUE((pack.make_field<0>().all()));
  EXPECT_TRUE((pack.make_field<1>().test(1)));
  EXPECT_TRUE((pack.make_field<2>().reset(1).none()));
}

TEST_F(bitfields_test, pack_field_eq) {
  au::Pack<1, 2, 1, 2> pack;
  pack.set(0b010101);
  auto field = pack.make_field<0>();
  EXPECT_TRUE(field == pack.make_field<0>());
  EXPECT_FALSE(field == pack.make_field<2>());
  pack.make_field<2>().set(0);
  EXPECT_TRUE(field == pack.make_field<2>());
}

TEST_F(bitfields_test, pack_field_less) {
  au::Pack<1, 2, 1, 2> pack;
  pack.set(0b010101);
  auto field = pack.make_field<1>();
  EXPECT_FALSE(field < pack.make_field<3>());
  pack.make_field<3>().set(1);
  EXPECT_TRUE(field < pack.make_field<3>());
}

TEST_F(bitfields_test, pack_set) {
  using pack_t = au::Pack<1, 2, 3>;
  using field_t = decltype(std::declval<pack_t>().make_field<1>());

  pack_t pack;
  pack.set(0b010101);
  pack.set<field_t>(0);
  EXPECT_EQ(0b010111, pack.get());
}

TEST_F(bitfields_test, pack_reset) {
  using pack_t = au::Pack<1, 2, 3>;
  using field_t = decltype(std::declval<pack_t>().make_field<1>());

  pack_t pack;
  pack.set(0b010101);
  pack.reset<field_t>(1);
  EXPECT_EQ(0b010001, pack.get());
}
}
