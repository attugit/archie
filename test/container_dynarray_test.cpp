#include <archie/utils/containers/dynamic_array.h>
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

using darray = cont::dynamic_array<resource>;

void canDefaultConstructDynamicArray() {
  darray da;
  EXPECT_EQ(0u, da.capacity());
  EXPECT_TRUE(da.empty());
}

void canCreateDynamicArrayWithGivenCapacity() {
  darray da1(1);
  darray da3(3);
  EXPECT_EQ(1u, da1.capacity());
  EXPECT_TRUE(da1.empty());

  EXPECT_EQ(3u, da3.capacity());
  EXPECT_TRUE(da3.empty());
}

void canMoveConstructDynamicArray() {
  darray da1(1);
  {
    EXPECT_EQ(1u, da1.capacity());
    EXPECT_TRUE(da1.empty());
  }

  darray da(std::move(da1));
  EXPECT_EQ(0u, da1.capacity());
  EXPECT_TRUE(da1.empty());
  EXPECT_EQ(1u, da.capacity());
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
    EXPECT_EQ(4u, orig.capacity());
    EXPECT_EQ(3u, orig.size());
    EXPECT_EQ(1u, copy.capacity());
    EXPECT_TRUE(copy.full());
  }

  copy = std::move(orig);
  EXPECT_EQ(0u, orig.capacity());
  EXPECT_EQ(4u, copy.capacity());
  EXPECT_EQ(3u, copy.size());
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
    EXPECT_EQ(orig.capacity(), copy.capacity());
    EXPECT_EQ(orig.size(), copy.size());
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
    EXPECT_EQ(orig.capacity(), copy.capacity());
    EXPECT_EQ(orig.size(), copy.size());
    EXPECT_NE(orig.data(), copy.data());
    EXPECT_EQ(orig[0], copy[0]);
    EXPECT_EQ(orig[1], copy[1]);
    EXPECT_EQ(orig[2], copy[2]);
  }

  darray empty;
  copy = empty;
  {
    EXPECT_EQ(0u, copy.capacity());
    EXPECT_TRUE(copy.empty());
  }
}

void canConstructAndAssignWithInitializerList() {
  darray da = {5, 7, 11, 13};
  {
    EXPECT_EQ(4u, da.capacity());
    EXPECT_EQ(4u, da.size());
    EXPECT_EQ(5, da[0]);
    EXPECT_EQ(7, da[1]);
    EXPECT_EQ(11, da[2]);
    EXPECT_EQ(13, da[3]);
  }

  da = {17, 19, 23, 29, 31};
  {
    EXPECT_EQ(5u, da.capacity());
    EXPECT_EQ(5u, da.size());
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
    EXPECT_EQ(1u, da.capacity());
    EXPECT_EQ(0u, da.size());
    EXPECT_TRUE(da.empty());
  }

  da.emplace_back(7);
  EXPECT_EQ(1u, da.capacity());
  EXPECT_EQ(1u, da.size());
  EXPECT_FALSE(da.empty());
  EXPECT_EQ(7, da[0]);
}

void canPopBackElement() {
  darray da(1);
  {
    EXPECT_EQ(1u, da.capacity());
    da.emplace_back(7);
    EXPECT_EQ(1u, da.capacity());
    EXPECT_EQ(1u, da.size());
    EXPECT_FALSE(da.empty());
  }

  da.pop_back();
  EXPECT_EQ(1u, da.capacity());
  EXPECT_EQ(0u, da.size());
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
    EXPECT_EQ(3u, da.size());
    EXPECT_TRUE(da.full());
    EXPECT_EQ(7, da[0]);
    EXPECT_EQ(11, da[1]);
    EXPECT_EQ(13, da[2]);
  }

  da.erase(da.begin());
  {
    EXPECT_EQ(2u, da.size());
    EXPECT_FALSE(da.full());
    EXPECT_EQ(11, da[0]);
    EXPECT_EQ(13, da[1]);
  }

  da.erase(da.begin() + 1);
  {
    EXPECT_EQ(1u, da.size());
    EXPECT_EQ(11, da[0]);
  }

  da.erase(da.begin());
  EXPECT_EQ(3u, da.capacity());
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
  auto size = da.size();
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
  auto size = da.size();
  auto capacity = da.capacity();
  auto first = da.release();
  EXPECT_EQ(0u, da.capacity());

  darray acc(0, alloc);
  EXPECT_EQ(0u, acc.capacity());
  {
    acc.acquire(first, size, capacity);
    EXPECT_EQ(7u, acc.capacity());
    EXPECT_EQ(4u, acc.size());
    EXPECT_EQ(5, acc[0]);
    EXPECT_EQ(7, acc[1]);
    EXPECT_EQ(11, acc[2]);
    EXPECT_EQ(13, acc[3]);
  }
}

template <typename Tp, typename Alloc = std::allocator<Tp>>
using sbo = cont::dynamic_array<Tp, Alloc, cont::enable_sbo>;

template <typename Tp, std::size_t Stock = 0u, typename Alloc = std::allocator<Tp>>
using rasbo = cont::dynamic_array<Tp, Alloc, cont::enable_ra_sbo, Stock>;

void canUseSboArray() {
  sbo<resource> sa;
  sbo<resource> other(4);
  {
    EXPECT_EQ(2u, sa.capacity());
    EXPECT_EQ(0u, sa.size());
    other.emplace_back(7);
    other.emplace_back(11);
    other.emplace_back(13);
  }

  sa.emplace_back(5);
  {
    EXPECT_EQ(1u, sa.size());
    EXPECT_EQ(5, sa[0]);
  }

  sa = std::move(other);
  {
    EXPECT_EQ(2u, other.capacity());
    EXPECT_EQ(0u, other.size());
    EXPECT_EQ(4u, sa.capacity());
    EXPECT_EQ(3u, sa.size());
    EXPECT_EQ(7, sa[0]);
    EXPECT_EQ(11, sa[1]);
    EXPECT_EQ(13, sa[2]);
  }
  sbo<resource> st;
  sa = std::move(st);
  {
    EXPECT_EQ(2u, sa.capacity());
    EXPECT_EQ(0u, sa.size());
  }
  {
    sbo<resource> acc;
    acc.emplace_back(5);
    auto size = acc.size();
    auto capacity = acc.capacity();
    auto ptr = acc.release();
    sa.acquire(ptr, size, capacity);
  }
  {
    EXPECT_EQ(2u, sa.capacity());
    EXPECT_EQ(1u, sa.size());
    EXPECT_EQ(5, sa[0]);
  }
}

void canUseRaSboArray() {
  rasbo<resource> sa;
  rasbo<resource> other(4);
  {
    EXPECT_EQ(1u, sa.capacity());
    EXPECT_EQ(0u, sa.size());
    other.emplace_back(7);
    other.emplace_back(11);
    other.emplace_back(13);
  }

  sa.emplace_back(5);
  {
    EXPECT_EQ(1u, sa.size());
    EXPECT_EQ(5, sa[0]);
  }

  sa = std::move(other);
  {
    EXPECT_EQ(1u, other.capacity());
    EXPECT_EQ(0u, other.size());
    EXPECT_EQ(4u, sa.capacity());
    EXPECT_EQ(3u, sa.size());
    EXPECT_EQ(7, sa[0]);
    EXPECT_EQ(11, sa[1]);
    EXPECT_EQ(13, sa[2]);
  }
  rasbo<resource> acc;
  acc.emplace_back(5);
  auto size = acc.size();
  auto capacity = acc.capacity();
  auto ptr = acc.release();
  sa.acquire(ptr, size, capacity);
  {
    EXPECT_EQ(1u, sa.capacity());
    EXPECT_EQ(1u, sa.size());
    EXPECT_EQ(5, sa[0]);
  }
}

void canUseRaSboArrayWithStock() {
  rasbo<resource, 8u> sa;
  rasbo<resource, 8u> other(10);
  {
    EXPECT_EQ(8u, sa.capacity());
    sa.emplace_back(3);
    EXPECT_EQ(1u, sa.size());
    EXPECT_EQ(10u, other.capacity());
    other.emplace_back(5);
    other.emplace_back(7);
    other.emplace_back(11);
    other.emplace_back(13);
    other.emplace_back(17);
    other.emplace_back(19);
    other.emplace_back(23);
    other.emplace_back(29);
    other.emplace_back(31);
    EXPECT_EQ(9u, other.size());
  }
  sa = std::move(other);
  {
    EXPECT_EQ(8u, other.capacity());
    EXPECT_EQ(0u, other.size());
    EXPECT_EQ(10u, sa.capacity());
    EXPECT_EQ(9u, sa.size());
  }
}

/**
 * Test plan:
 * func: release/acquire/move/copy
 * A  class  src    dst
 *  1 sbo    empty  empty
 *  2 sbo    empty  stack
 *  3 sbo    empty  heap
 *  4 sbo    stack  empty
 *  5 sbo    stack  stack
 *  6 sbo    stack  heap
 *  7 sbo    heap   empty
 *  8 sbo    heap   stack
 *  9 sbo    heap   heap
 *
 * B  class  src    dst
 *  1 rasbo  empty  empty
 *  2 rasbo  empty  stack
 *  3 rasbo  empty  heap
 *  4 rasbo  stack  empty
 *  5 rasbo  stack  stack
 *  6 rasbo  stack  heap
 *  7 rasbo  heap   empty
 *  8 rasbo  heap   stack
 *  9 rasbo  heap   heap
 *
 * C  class  src    dst
 *  1 stock  empty  empty
 *  2 stock  empty  stack
 *  3 stock  empty  heap
 *  4 stock  stack  empty
 *  5 stock  stack  stack
 *  6 stock  stack  heap
 *  7 stock  heap   empty
 *  8 stock  heap   stack
 *  9 stock  heap   heap
 */

int main() {
  canDefaultConstructDynamicArray();
  canCreateDynamicArrayWithGivenCapacity();
  canMoveConstructDynamicArray();
  canMoveAssign();
  canCopyConstruct();
  canCopyAssign();
  canConstructAndAssignWithInitializerList();
  canEmplaceBackElement();
  canPopBackElement();
  canUseBeginAndEnd();
  canEraseElement();
  canReleaseContent();
  canAcquire();
  canUseSboArray();
  canUseRaSboArray();
  canUseRaSboArrayWithStock();
  return 0;
}
