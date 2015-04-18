#include <memory>

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

        void destroy_storage() noexcept { deallocate(start_, end_of_storage_ - start_); }

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

      explicit dynamic_array(allocator_type const& a) : impl_(a) {}

      dynamic_array(size_type n, allocator_type const& a) : impl_(a) { impl_.create_storage(n); }

      dynamic_array(dynamic_array&& x) { impl_.swap(x.impl_); }

      dynamic_array& operator=(dynamic_array&&) = default;

      dynamic_array(dynamic_array const&) {}

      dynamic_array& operator=(dynamic_array const&) { return *this; }

      dynamic_array(std::initializer_list<value_type>) {}

      dynamic_array& operator=(std::initializer_list<value_type>) {}

      template <typename InputIterator>
      dynamic_array(InputIterator, InputIterator) {}

      ~dynamic_array() noexcept { impl_.destroy_storage(); }

      size_type capacity() const { return size_type(impl_.end_of_storage_ - impl_.start_); }

      size_type size() const { return size_type(impl_.finish_ - impl_.start_); }

      bool empty() const { return size() == 0u; }
    };
  }
}
}

#include <archie/utils/test.h>
#include <memory>
namespace cont = archie::utils::containers;

void canDefaultConstructDynamicArray() {
  cont::dynamic_array<int> da;
  EXPECT_EQ(0u, da.capacity());
  EXPECT_TRUE(da.empty());
}

void canCreateDynamicArrayWithGivenCapacity() {
  cont::dynamic_array<int> da1(1);
  EXPECT_EQ(1u, da1.capacity());
  EXPECT_TRUE(da1.empty());

  cont::dynamic_array<int> da3(3);
  EXPECT_EQ(3u, da3.capacity());
  EXPECT_TRUE(da3.empty());
}

void canMoveConstructDynamicArray() {
  cont::dynamic_array<int> da1(1);
  EXPECT_EQ(1u, da1.capacity());
  EXPECT_TRUE(da1.empty());

  cont::dynamic_array<int> da(std::move(da1));
  EXPECT_EQ(0u, da1.capacity());
  EXPECT_TRUE(da1.empty());
  EXPECT_EQ(1u, da.capacity());
  EXPECT_TRUE(da.empty());
}

int main() {
  canDefaultConstructDynamicArray();
  canCreateDynamicArrayWithGivenCapacity();
  canMoveConstructDynamicArray();
  return 0;
}
