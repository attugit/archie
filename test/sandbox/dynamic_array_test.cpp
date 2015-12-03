#include <archie/container/stack_buffer.hpp>
#include <archie/container/heap_buffer.hpp>
#include <resource.hpp>
#include <catch.hpp>
namespace {
using namespace archie;
TEST_CASE("stack_buffer", "[array]") {
  enum { stack_size = 7 };
  using sut = stack_buffer<test::resource, stack_size>;
  using value_t = typename sut::value_type;
  SECTION("ctor") {
    sut buff;
    REQUIRE(buff.capacity() == stack_size);
    REQUIRE(buff.empty());
  }
  SECTION("emplace_back") {
    sut buff;
    buff.emplace_back(1);
    REQUIRE(!buff.empty());
    REQUIRE(buff.size() == 1);
    REQUIRE(buff[0] == 1);
  }
  SECTION("initializer_list") {
    sut buff = {value_t(1), value_t(2), value_t(3), value_t(4), value_t(5)};
    REQUIRE(buff.size() == 5);
    REQUIRE(buff[0] == 1);
    REQUIRE(buff[1] == 2);
    REQUIRE(buff[2] == 3);
    REQUIRE(buff[3] == 4);
    REQUIRE(buff[4] == 5);
  }
  SECTION("copy ctor") {
    sut const orig = {value_t(1), value_t(2), value_t(3)};
    sut cpy{orig};
    REQUIRE(cpy == orig);
  }
  SECTION("move ctor") {
    sut const ref = {value_t(1), value_t(2), value_t(3)};
    sut orig{ref};
    sut cpy{std::move(orig)};
    REQUIRE(cpy == ref);
  }
  SECTION("copy assignment") {
    sut const orig = {value_t(1), value_t(2), value_t(3)};
    sut cpy0;
    sut cpy1 = {value_t(4)};
    sut cpy2 = {value_t(5), value_t(6), value_t(7)};
    sut cpy3 = {value_t(8), value_t(9), value_t(10), value_t(11)};

    cpy0 = orig;
    REQUIRE(cpy0 == orig);
    cpy1 = orig;
    REQUIRE(cpy1 == orig);
    cpy2 = orig;
    REQUIRE(cpy2 == orig);
    cpy3 = orig;
    REQUIRE(cpy3 == orig);
    cpy3 = cpy3;
    REQUIRE(cpy3 == orig);
  }
  SECTION("eq cmp") {
    sut lhs = {value_t(1), value_t(2), value_t(3)};
    sut rhs;
    rhs.emplace_back(1);
    rhs.emplace_back(2);
    rhs.emplace_back(3);
    REQUIRE(lhs == rhs);
    rhs.emplace_back(4);
    REQUIRE(lhs != rhs);
  }
}
TEST_CASE("heap_buffer", "[array]") {
  using sut = heap_buffer<test::resource>;
  using value_t = typename sut::value_type;
  SECTION("default ctor") {
    sut buff;
    REQUIRE(buff.capacity() == 0);
    REQUIRE(buff.empty());
  }
  SECTION("ctor") {
    sut buff(7);
    REQUIRE(buff.capacity() == 7);
    REQUIRE(buff.empty());
  }
  SECTION("emplace_back") {
    sut buff(7);
    buff.emplace_back(1);
    REQUIRE(!buff.empty());
    REQUIRE(buff.size() == 1);
    REQUIRE(buff[0] == 1);
  }
  SECTION("initializer_list") {
    sut buff = {value_t(1), value_t(2), value_t(3)};
    REQUIRE(buff.size() == 3);
    REQUIRE(buff[0] == 1);
    REQUIRE(buff[1] == 2);
    REQUIRE(buff[2] == 3);
  }
  SECTION("copy ctor") {
    sut const orig = {value_t(1), value_t(2), value_t(3)};
    sut cpy{orig};
    REQUIRE(cpy == orig);
  }
  SECTION("move ctor") {
    sut const ref = {value_t(1), value_t(2), value_t(3)};
    sut orig{ref};
    sut cpy{std::move(orig)};
    REQUIRE(cpy == ref);
    REQUIRE(orig.empty());
  }
  SECTION("copy assignment") {
    sut const orig = {value_t(1), value_t(2), value_t(3)};
    sut cpy0;
    sut cpy1 = {value_t(4)};
    sut cpy2 = {value_t(5), value_t(6), value_t(7)};
    sut cpy3 = {value_t(8), value_t(9), value_t(10), value_t(11)};

    cpy0 = orig;
    REQUIRE(cpy0 == orig);
    cpy1 = orig;
    REQUIRE(cpy1 == orig);
    cpy2 = orig;
    REQUIRE(cpy2 == orig);
    cpy3 = orig;
    REQUIRE(cpy3 == orig);
    cpy3 = cpy3;
    REQUIRE(cpy3 == orig);
  }
  SECTION("eq cmp") {
    sut lhs = {value_t(1), value_t(2), value_t(3)};
    sut rhs(5);
    rhs.emplace_back(1);
    rhs.emplace_back(2);
    rhs.emplace_back(3);
    REQUIRE(lhs == rhs);
    rhs.emplace_back(4);
    REQUIRE(lhs != rhs);
  }
}
TEST_CASE("mixed_buffer", "[array]") {
  enum { stack_size = 7 };
  using sut = mixed_buffer<test::resource, stack_size>;
  using value_t = typename sut::value_type;
  SECTION("default ctor") {
    sut buff;
    REQUIRE(buff.empty());
    REQUIRE(buff.capacity() == stack_size);
  }
  SECTION("ctor") {
    sut buff_s(stack_size - 1);
    REQUIRE(buff_s.empty());
    REQUIRE(buff_s.capacity() == stack_size);

    sut buff_h(stack_size + 1);
    REQUIRE(buff_h.empty());
    REQUIRE(buff_h.capacity() == stack_size + 1);
  }
  SECTION("initializer_list") {
    sut buff_s = {value_t(1), value_t(2), value_t(3), value_t(4), value_t(5)};
    REQUIRE(buff_s.size() == 5);
    REQUIRE(buff_s.capacity() == stack_size);
    REQUIRE(buff_s[0] == 1);
    REQUIRE(buff_s[1] == 2);
    REQUIRE(buff_s[2] == 3);
    REQUIRE(buff_s[3] == 4);
    REQUIRE(buff_s[4] == 5);

    sut buff_h = {value_t(1),
                  value_t(2),
                  value_t(3),
                  value_t(4),
                  value_t(5),
                  value_t(6),
                  value_t(7),
                  value_t(8)};
    REQUIRE(buff_h.size() == 8);
    REQUIRE(buff_h.capacity() == 8);
    REQUIRE(buff_h[0] == 1);
    REQUIRE(buff_h[1] == 2);
    REQUIRE(buff_h[2] == 3);
    REQUIRE(buff_h[3] == 4);
    REQUIRE(buff_h[4] == 5);
    REQUIRE(buff_h[5] == 6);
    REQUIRE(buff_h[6] == 7);
    REQUIRE(buff_h[7] == 8);
  }
  SECTION("copy ctor") {
    sut const orig_s = {value_t(1), value_t(2), value_t(3)};
    sut cpy_s{orig_s};
    REQUIRE(cpy_s == orig_s);

    sut const orig_h = {value_t(1),
                        value_t(2),
                        value_t(3),
                        value_t(4),
                        value_t(5),
                        value_t(6),
                        value_t(7),
                        value_t(8)};
    sut cpy_h{orig_h};
    REQUIRE(cpy_h == orig_h);
  }
  SECTION("move ctor") {
    sut const ref_s = {value_t(1), value_t(2), value_t(3)};
    sut orig_s{ref_s};
    sut cpy_s{std::move(orig_s)};
    REQUIRE(cpy_s == ref_s);
    REQUIRE(orig_s.capacity() == stack_size);
    REQUIRE(orig_s.size() == ref_s.size());

    sut const ref_h = {value_t(1),
                       value_t(2),
                       value_t(3),
                       value_t(4),
                       value_t(5),
                       value_t(6),
                       value_t(7),
                       value_t(8)};
    sut orig_h{ref_h};
    REQUIRE(orig_h.capacity() > stack_size);
    sut cpy_h{std::move(orig_h)};
    REQUIRE(cpy_h == ref_h);
    REQUIRE(orig_h.capacity() == stack_size);
    REQUIRE(orig_h.empty());
  }
  auto const make_sut = [](sut::size_type S) {
    sut ret(S);
    for (auto idx = 0u; idx < S; ++idx) { ret.emplace_back(static_cast<int>(idx)); }
    return ret;
  };
  SECTION("copy assignment stack buffer") {
    sut const orig_s = {value_t(100), value_t(101), value_t(102)};
    sut cpy0;
    sut cpy1 = make_sut(1);
    sut cpy2 = make_sut(3);
    sut cpy3 = make_sut(6);
    sut cpy4 = make_sut(8);
    sut cpy5 = make_sut(9);

    cpy0 = orig_s;
    REQUIRE(cpy0 == orig_s);
    cpy1 = orig_s;
    REQUIRE(cpy1 == orig_s);
    cpy2 = orig_s;
    REQUIRE(cpy2 == orig_s);
    cpy3 = orig_s;
    REQUIRE(cpy3 == orig_s);
    cpy4 = orig_s;
    REQUIRE(cpy4 == orig_s);
    cpy5 = orig_s;
    REQUIRE(cpy5 == orig_s);

    cpy5 = cpy5;
    REQUIRE(cpy5 == orig_s);
  }
  SECTION("copy assignment heap buffer") {
    sut const orig_s = {value_t(100),
                        value_t(101),
                        value_t(102),
                        value_t(103),
                        value_t(104),
                        value_t(105),
                        value_t(106),
                        value_t(107)};
    sut cpy0;
    sut cpy1 = make_sut(1);
    sut cpy2 = make_sut(3);
    sut cpy3 = make_sut(6);
    sut cpy4 = make_sut(8);
    sut cpy5 = make_sut(9);

    cpy0 = orig_s;
    REQUIRE(cpy0 == orig_s);
    cpy1 = orig_s;
    REQUIRE(cpy1 == orig_s);
    cpy2 = orig_s;
    REQUIRE(cpy2 == orig_s);
    cpy3 = orig_s;
    REQUIRE(cpy3 == orig_s);
    cpy4 = orig_s;
    REQUIRE(cpy4 == orig_s);
    cpy5 = orig_s;
    REQUIRE(cpy5 == orig_s);

    cpy5 = cpy5;
    REQUIRE(cpy5 == orig_s);
  }

  SECTION("eq cmp") {
    sut lhs = {value_t(1), value_t(2), value_t(3)};
    sut rhs(5);
    rhs.emplace_back(1);
    rhs.emplace_back(2);
    rhs.emplace_back(3);
    REQUIRE(lhs == rhs);
    rhs.emplace_back(4);
    REQUIRE(lhs != rhs);
  }
}
}
