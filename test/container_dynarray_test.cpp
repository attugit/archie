#include <memory>
#include <tuple>
#include <type_traits>

#define IMPLEMENT_ME std::terminate();

namespace archie {
namespace utils {
  namespace containers {
    template <typename Tp, typename Alloc = std::allocator<Tp>>
    struct dynamic_array {
      using allocator_type = Alloc;
      using pointer = typename allocator_type::pointer;
      using const_pointer = typename allocator_type::const_pointer;
      using value_type = typename allocator_type::value_type;
      using reference = typename allocator_type::reference;
      using const_reference = typename allocator_type::const_reference;
      using size_type = typename allocator_type::size_type;
      using difference_type = typename allocator_type::difference_type;

      using iterator = __gnu_cxx::__normal_iterator<pointer, dynamic_array>;
      using const_iterator = __gnu_cxx::__normal_iterator<const_pointer, dynamic_array>;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
      using tp_alloc = typename allocator_type::template rebind<Tp>::other;
      struct impl : private tp_alloc {
        using tp_alloc::allocate;
        using tp_alloc::deallocate;
        using tp_alloc::construct;
        using tp_alloc::destroy;

        pointer start_ = nullptr;
        pointer finish_ = nullptr;
        pointer end_of_storage_ = nullptr;

        impl() noexcept = default;

        explicit impl(tp_alloc const& a) noexcept : tp_alloc(a),
                                                    start_(nullptr),
                                                    finish_(nullptr),
                                                    end_of_storage_(nullptr) {}

        void create_storage(size_type n) {
          if (n != size_type(end_of_storage_ - start_)) {
            start_ = (n != 0u) ? allocate(n) : nullptr;
            finish_ = start_;
            end_of_storage_ = start_ + n;
          }
        }

        void destroy_storage() noexcept {
          deallocate(start_, end_of_storage_ - start_);
          start_ = nullptr;
          finish_ = nullptr;
          end_of_storage_ = nullptr;
        }

        tp_alloc& get_allocator() noexcept { return static_cast<tp_alloc&>(*this); }

        tp_alloc const& get_allocator() const noexcept {
          return static_cast<tp_alloc const&>(*this);
        }

        void swap(impl& x) noexcept {
          std::swap(start_, x.start_);
          std::swap(finish_, x.finish_);
          std::swap(end_of_storage_, x.end_of_storage_);
          std::swap(get_allocator(), x.get_allocator());
        }
      };

      impl impl_;

    public:
      dynamic_array() = default;

      explicit dynamic_array(size_type n) { impl_.create_storage(n); }

      dynamic_array(size_type n, allocator_type const& a) : impl_(a) { impl_.create_storage(n); }

      dynamic_array(dynamic_array&& x) noexcept { impl_.swap(x.impl_); }

      dynamic_array& operator=(dynamic_array&& x) noexcept {
        clear();
        if (capacity() != 0u) impl_.destroy_storage();
        impl_.swap(x.impl_);
        return *this;
      }

      dynamic_array(dynamic_array const& x) : dynamic_array(x.capacity()) {
        std::copy(std::begin(x), std::end(x), std::back_inserter(*this));
      }

      dynamic_array& operator=(dynamic_array const& x) {
        clear();
        if (capacity() != x.capacity() && capacity() != 0u) {
          impl_.destroy_storage();
          impl_.create_storage(x.capacity());
        }
        std::copy(std::begin(x), std::end(x), std::back_inserter(*this));
        return *this;
      }

      dynamic_array(std::initializer_list<value_type> list)
          : dynamic_array(std::begin(list), std::end(list)) {}

      dynamic_array& operator=(std::initializer_list<value_type>) {
        IMPLEMENT_ME
        return *this;
      }

      template <typename InputIterator>
      dynamic_array(InputIterator first, InputIterator last)
          : dynamic_array(std::distance(first, last)) {
        std::copy(first, last, std::back_inserter(*this));
      }

      ~dynamic_array() noexcept {
        clear();
        if (capacity() != 0u) impl_.destroy_storage();
      }

      size_type capacity() const noexcept {
        return size_type(impl_.end_of_storage_ - impl_.start_);
      }

      size_type size() const noexcept { return size_type(impl_.finish_ - impl_.start_); }

      bool empty() const noexcept { return cbegin() == cend(); }

      bool full() const noexcept { return impl_.finish_ == impl_.end_of_storage_; }

      template <typename... Args>
      void emplace_back(Args&&... args) noexcept(
          std::is_nothrow_constructible<value_type, Args...>::value) {
        impl_.construct(impl_.finish_, std::forward<Args>(args)...);
        ++impl_.finish_;
      }

      void push_back(const_reference x) noexcept(
          std::is_nothrow_copy_constructible<value_type>::value) {
        emplace_back(x);
      }

      void push_back(value_type&& x) noexcept(
          std::is_nothrow_move_constructible<value_type>::value) {
        emplace_back(std::move(x));
      }

      void pop_back() noexcept { impl_.destroy(--impl_.finish_); }

      void erase(iterator pos) noexcept(std::is_nothrow_move_assignable<value_type>::value) {
        std::move(pos + 1, end(), pos);
        pop_back();
      }

      void clear() noexcept {
        while (!empty()) pop_back();
      }

      const_reference operator[](size_type n) const noexcept { return *(impl_.start_ + n); }

      reference operator[](size_type n) noexcept { return *(impl_.start_ + n); }

      iterator begin() noexcept { return iterator(impl_.start_); }

      iterator end() noexcept { return iterator(impl_.finish_); }

      const_iterator cbegin() const noexcept { return const_iterator(impl_.start_); }

      const_iterator begin() const noexcept { return cbegin(); }

      const_iterator cend() const noexcept { return const_iterator(impl_.finish_); }

      const_iterator end() const noexcept { return cend(); }

      pointer data() noexcept { return impl_.start_; }

      const_pointer data() const noexcept { return impl_.start_; }

      std::tuple<pointer, pointer, pointer> release() noexcept {
        auto ret = std::make_tuple(impl_.start_, impl_.finish_, impl_.end_of_storage_);
        impl_.start_ = nullptr;
        impl_.finish_ = nullptr;
        impl_.end_of_storage_ = nullptr;
        return ret;
      }

      allocator_type& get_allocator() noexcept {
        return static_cast<allocator_type&>(impl_.get_allocator());
      }

      allocator_type const& get_allocator() const noexcept {
        return static_cast<allocator_type const&>(impl_.get_allocator());
      }
    };
  }
}
}

#include <archie/utils/test.h>
#include <memory>
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
  darray::pointer first, last, end;
  std::tie(first, last, end) = da.release();
  EXPECT_EQ(4, last - first);
  alloc.deallocate(first, end - first);
}

int main() {
  canDefaultConstructDynamicArray();
  canCreateDynamicArrayWithGivenCapacity();
  canMoveConstructDynamicArray();
  canMoveAssign();
  canCopyConstruct();
  canCopyAssign();
  canEmplaceBackElement();
  canPopBackElement();
  canUseBeginAndEnd();
  canEraseElement();
  canReleaseContent();
  return 0;
}
