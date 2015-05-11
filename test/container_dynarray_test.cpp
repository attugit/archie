#include <archie/utils/containers/dynamic_array.h>
#include <archie/utils/containers/capacity.h>
#include <archie/utils/containers/size.h>
#include <archie/utils/test.h>
namespace cont = archie::utils::containers;

struct resource {
  resource(int i) : handle(new int(i)) {}
  resource(resource const& x) : handle(new int(*(x.handle))) {}
  resource(resource&& x) noexcept : handle(x.handle) { x.handle = nullptr; }
  resource& operator=(resource const& x) {
    if (handle == nullptr)
      handle = new int(*x.handle);
    else
      *handle = *(x.handle);
    return *this;
  }
  resource& operator=(resource&& x) noexcept {
    if (handle) delete handle;
    handle = x.handle;
    x.handle = nullptr;
    return *this;
  }
  ~resource() noexcept {
    if (handle) delete handle;
  }
  int* handle = nullptr;
  friend bool operator==(resource const& lhs, resource const& rhs) noexcept {
    return *lhs.handle == *rhs.handle;
  }
  friend bool operator==(resource const& r, int i) noexcept { return *r.handle == i; }
  friend bool operator==(int i, resource const& r) noexcept { return r == i; }
  friend bool operator!=(int i, resource const& r) noexcept { return !(r == i); }
  friend bool operator!=(resource const& r, int i) noexcept { return !(r == i); }
};

template <typename SuT>
struct TestFixture {
  using darray = SuT;
  using size_type = typename darray::size_type;
  constexpr static size_type stock_size() noexcept {
    return size_type(reinterpret_cast<darray*>(0)->stock().size());
  }
  static void run() {
    TestFixture tf;
    tf.canDefaultConstructDynamicArray();
    tf.canCreateDynamicArrayWithGivenCapacity();
    tf.canMoveConstructDynamicArray();
    tf.canMoveAssign();
    tf.canCopyConstruct();
    tf.canCopyAssign();
    tf.canConstructAndAssignWithInitializerList();
    tf.canEmplaceBackElement();
    tf.canPopBackElement();
    tf.canUseBeginAndEnd();
    tf.canEraseElement();
    tf.canReleaseContent();
    tf.canAcquire();
  }

  void canDefaultConstructDynamicArray() {
    darray da;
    EXPECT_EQ(stock_size(), cont::capacity(da));
    EXPECT_TRUE(da.empty());
  }

  void canCreateDynamicArrayWithGivenCapacity() {
    darray da1(1);
    darray da3(3);
    EXPECT_TRUE(cont::capacity(da1) >= 1u);
    EXPECT_TRUE(da1.empty());

    EXPECT_TRUE(cont::capacity(da3) >= 3u);
    EXPECT_TRUE(da3.empty());
  }

  void canMoveConstructDynamicArray() {
    darray da1(1);
    {
      EXPECT_TRUE(cont::capacity(da1) >= 1u);
      EXPECT_TRUE(da1.empty());
    }

    darray da(std::move(da1));
    EXPECT_EQ(stock_size(), cont::capacity(da1));
    EXPECT_TRUE(da1.empty());
    EXPECT_TRUE(cont::capacity(da) >= 1u);
    EXPECT_TRUE(da.empty());
  }

  void canMoveAssign() {
    darray orig(4);
    darray copy(1);
    {
      orig.emplace_back(5);
      orig.emplace_back(7);
      orig.emplace_back(11);
      copy.emplace_back(13);
      EXPECT_TRUE(cont::capacity(orig) >= 4u);
      EXPECT_EQ(3u, cont::size(orig));
      EXPECT_TRUE(cont::capacity(copy) >= 1u);
    }

    copy = std::move(orig);
    EXPECT_EQ(stock_size(), cont::capacity(orig));
    EXPECT_TRUE(cont::capacity(copy) >= 4u);
    EXPECT_EQ(3u, cont::size(copy));
    EXPECT_EQ(5, copy[0]);
    EXPECT_EQ(7, copy[1]);
    EXPECT_EQ(11, copy[2]);
  }

  void canCopyConstruct() {
    darray orig(4);
    {
      orig.emplace_back(5);
      orig.emplace_back(7);
      orig.emplace_back(11);
    }

    darray copy(orig);
    {
      EXPECT_EQ(cont::capacity(orig), cont::capacity(copy));
      EXPECT_EQ(cont::size(orig), cont::size(copy));
      EXPECT_NE(orig.data(), copy.data());
      EXPECT_EQ(orig[0], copy[0]);
      EXPECT_EQ(orig[1], copy[1]);
      EXPECT_EQ(orig[2], copy[2]);
    }
  }

  void canCopyAssign() {
    darray orig(4);
    darray copy(1);
    {
      orig.emplace_back(5);
      orig.emplace_back(7);
      orig.emplace_back(11);
      copy.emplace_back(13);
    }

    copy = orig;
    {
      EXPECT_EQ(cont::capacity(orig), cont::capacity(copy));
      EXPECT_EQ(cont::size(orig), cont::size(copy));
      EXPECT_NE(orig.data(), copy.data());
      EXPECT_EQ(orig[0], copy[0]);
      EXPECT_EQ(orig[1], copy[1]);
      EXPECT_EQ(orig[2], copy[2]);
    }

    darray empty;
    copy = empty;
    {
      EXPECT_EQ(stock_size(), cont::capacity(copy));
      EXPECT_TRUE(copy.empty());
    }
  }

  void canConstructAndAssignWithInitializerList() {
    darray da = {5, 7, 11, 13};
    {
      EXPECT_TRUE(cont::capacity(da) >= 4u);
      EXPECT_EQ(4u, cont::size(da));
      EXPECT_EQ(5, da[0]);
      EXPECT_EQ(7, da[1]);
      EXPECT_EQ(11, da[2]);
      EXPECT_EQ(13, da[3]);
    }

    da = {17, 19, 23, 29, 31};
    {
      EXPECT_TRUE(cont::capacity(da) >= 5u);
      EXPECT_EQ(5u, cont::size(da));
      EXPECT_EQ(17, da[0]);
      EXPECT_EQ(19, da[1]);
      EXPECT_EQ(23, da[2]);
      EXPECT_EQ(29, da[3]);
      EXPECT_EQ(31, da[4]);
    }
  }

  void canEmplaceBackElement() {
    darray da(1);
    {
      EXPECT_TRUE(cont::capacity(da) >= 1u);
      EXPECT_EQ(0u, cont::size(da));
      EXPECT_TRUE(da.empty());
    }

    da.emplace_back(7);
    EXPECT_TRUE(cont::capacity(da) >= 1u);
    EXPECT_EQ(1u, cont::size(da));
    EXPECT_FALSE(da.empty());
    EXPECT_EQ(7, da[0]);
  }

  void canPopBackElement() {
    darray da(1);
    {
      EXPECT_TRUE(cont::capacity(da) >= 1u);
      da.emplace_back(7);
      EXPECT_TRUE(cont::capacity(da) >= 1u);
      EXPECT_EQ(1u, cont::size(da));
      EXPECT_FALSE(da.empty());
    }

    da.pop_back();
    EXPECT_TRUE(cont::capacity(da) >= 1u);
    EXPECT_EQ(0u, cont::size(da));
    EXPECT_TRUE(da.empty());
  }

  void canUseBeginAndEnd() {
    darray da0;
    darray da1(1);
    darray da2(2);
    {
      EXPECT_EQ(da0.begin(), da0.end());
      EXPECT_EQ(da1.begin(), da1.end());
      EXPECT_EQ(da2.begin(), da2.end());
    }

    da1.emplace_back(11);
    da2.emplace_back(13);
    {
      EXPECT_EQ(da1.begin() + 1, da1.end());
      EXPECT_EQ(da2.begin() + 1, da2.end());
    }

    da2.emplace_back(17);
    EXPECT_EQ(da2.begin() + 2, da2.end());
  }

  void canEraseElement() {
    darray da(3);
    {
      da.emplace_back(7);
      da.emplace_back(11);
      da.emplace_back(13);
      EXPECT_EQ(3u, cont::size(da));
      EXPECT_EQ(7, da[0]);
      EXPECT_EQ(11, da[1]);
      EXPECT_EQ(13, da[2]);
    }

    da.erase(da.begin());
    {
      EXPECT_EQ(2u, cont::size(da));
      EXPECT_FALSE(da.full());
      EXPECT_EQ(11, da[0]);
      EXPECT_EQ(13, da[1]);
    }

    da.erase(da.begin() + 1);
    {
      EXPECT_EQ(1u, cont::size(da));
      EXPECT_EQ(11, da[0]);
    }

    da.erase(da.begin());
    EXPECT_TRUE(cont::capacity(da) >= 3u);
    EXPECT_TRUE(da.empty());
  }

  void canReleaseContent() {
    darray da(7);
    {
      da.emplace_back(5);
      da.emplace_back(7);
      da.emplace_back(11);
      da.emplace_back(13);
    }

    auto alloc = da.get_allocator();
    auto size = cont::size(da);
    auto first = da.release();
    auto last = std::next(first, size);
    EXPECT_EQ(4, size);
    for (; first != last;) {
      --last;
      last->~resource();
    }
    alloc.deallocate(first, size);
  }

  void canAcquire() {
    darray da(7);
    {
      da.emplace_back(5);
      da.emplace_back(7);
      da.emplace_back(11);
      da.emplace_back(13);
    }

    auto alloc = da.get_allocator();
    auto size = cont::size(da);
    auto capacity = cont::capacity(da);
    auto first = da.release();
    EXPECT_EQ(stock_size(), cont::capacity(da));

    darray acc(0, alloc);
    EXPECT_EQ(stock_size(), cont::capacity(acc));
    {
      acc.acquire(first, size, capacity);
      EXPECT_EQ(7u, cont::capacity(acc));
      EXPECT_EQ(4u, cont::size(acc));
      EXPECT_EQ(5, acc[0]);
      EXPECT_EQ(7, acc[1]);
      EXPECT_EQ(11, acc[2]);
      EXPECT_EQ(13, acc[3]);
    }
  }
};

template <typename Tp, typename Alloc = std::allocator<Tp>>
using disbo = cont::dynamic_array<Tp, Alloc, cont::disable_sbo>;

template <typename Tp, typename Alloc = std::allocator<Tp>>
using ensbo = cont::dynamic_array<Tp, Alloc, cont::enable_sbo>;

template <typename Tp, std::size_t N = 0u, typename Alloc = std::allocator<Tp>>
using stock = cont::dynamic_array<Tp, Alloc, cont::enable_sbo, N>;

#include <vector>
#include <functional>

template <typename Tp>
struct setup {
  constexpr static auto stock_size() noexcept { return reinterpret_cast<Tp*>(0)->stock().size(); }
  static Tp make_data(std::size_t first, std::size_t size) {
    Tp data(size);
    while (size--) { data.emplace_back(first++); }
    return data;
  }
};

template <template <typename...> class SuT>
void verifySboMemoryManagement() {
  using sut_t = SuT<resource>;
  auto stock_size = setup<sut_t>::stock_size();
  using verif_t = sut_t;
  using testcase_t = std::function<void(sut_t, verif_t const&)>;
  auto make_check = [](sut_t const& expected) {
    return [exp = expected](sut_t const& dst) {
      EXPECT_EQ(cont::capacity(exp), cont::capacity(dst));
      EXPECT_EQ(cont::size(exp), cont::size(dst));
      for (auto idx = 0u; idx < cont::size(exp); ++idx) EXPECT_EQ(exp[idx], dst[idx]);
    };
  };
  auto tc_move = [make_check](sut_t sut, verif_t const& v) {
    auto check = make_check(v);
    auto cpy = v;
    sut = std::move(cpy);
    check(sut);
  };
  auto tc_release_acquire = [make_check](sut_t sut, verif_t const& v) {
    auto const size = cont::size(v);
    auto const capacity = cont::capacity(v);
    auto check = make_check(v);
    auto cpy = v;
    sut.acquire(cpy.release(), size, capacity);
    check(sut);
  };
  auto tc_copy = [make_check](sut_t sut, verif_t const& v) {
    auto check = make_check(v);
    sut = v;
    check(sut);
  };
  std::vector<testcase_t> testcases = {tc_move, tc_copy, tc_release_acquire};
  std::vector<sut_t> expected;
  std::vector<sut_t> data;
  {
    expected.emplace_back();
    data.emplace_back();
  }
  {
    expected.emplace_back(setup<sut_t>::make_data(3, stock_size));
    data.emplace_back(setup<sut_t>::make_data(11, stock_size));
  }
  {
    expected.emplace_back(setup<sut_t>::make_data(3, stock_size + 2));
    data.emplace_back(setup<sut_t>::make_data(11, stock_size + 2));
  }
  for (auto const& tc : testcases) {
    for (auto const& ex : expected) {
      for (auto const& dst : data) { tc(dst, ex); }
    }
  }
}

int main() {
  TestFixture<disbo<resource>>::run();
  TestFixture<ensbo<resource>>::run();
  TestFixture<stock<resource, 7u>>::run();
  verifySboMemoryManagement<ensbo>();
  return 0;
}
