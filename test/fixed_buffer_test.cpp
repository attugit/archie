#include <cstddef>
#include <utility>
#include <new>

namespace archie {
namespace utils {
  namespace containers {

    template <typename Tp, std::size_t N>
    struct stack_allocator {
      using value_type = Tp;
      using pointer = Tp*;
      using reference = Tp&;
      using const_reference = Tp const&;
      using const_pointer = Tp const*;
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;

      pointer allocate(size_type n) {
        auto s = taken_;
        taken_ += n;
        return &store.array[s];
      }
      void deallocate(pointer, size_type) {}
      size_type max_size() const { return N - taken_; }

    private:
      union storage {
        char raw[N * sizeof(value_type)];
        value_type array[N];
      };
      storage store;
      size_type taken_ = 0u;
    };

    template <typename Tp, std::size_t N,
              typename Alloc = stack_allocator<Tp, N>>
    struct fixed_buffer {
      using allocator_type = Alloc;
      using value_type = typename allocator_type::value_type;
      using pointer = typename allocator_type::pointer;
      using reference = typename allocator_type::reference;
      using const_reference = typename allocator_type::const_reference;
      using const_pointer = typename allocator_type::const_pointer;
      using size_type = typename allocator_type::size_type;
      using iterator = pointer;
      using const_iterator = const_pointer;

      explicit fixed_buffer(size_type n) : impl_() {
        while (n--) emplace_back();
      }

      template <typename Up>
      fixed_buffer(size_type n, Up const& ref)
          : impl_() {
        while (n--) emplace_back(ref);
      }

      fixed_buffer() : fixed_buffer(0u) {}
      ~fixed_buffer() {
        while (size()) {
          back().~value_type();
          --impl_.size_;
        }
      }

      const_reference operator[](size_type n) const {
        return *(impl_.begin_ + n);
      }
      const_reference front() const { return operator[](0); }
      const_iterator begin() const { return &front(); }
      const_iterator end() const { return begin() + size(); }

      reference operator[](size_type n) { return *(impl_.begin_ + n); }
      reference front() { return operator[](0); }
      reference back() { return operator[](size() - 1u); }
      iterator begin() { return &front(); }
      iterator end() { return begin() + size(); }

      size_type size() const { return impl_.size_; }

      template <typename... Us>
      void emplace_back(Us&&... us) {
        new (end()) value_type(std::forward<Us>(us)...);
        ++impl_.size_;
      }

    private:
      struct impl : allocator_type {
        using allocator_type::allocate;
        using allocator_type::deallocate;
        impl() : begin_(allocate(N)) {}
        ~impl() { deallocate(begin_, N); }
        pointer const begin_ = nullptr;
        size_type size_ = 0u;
      };
      impl impl_;
    };

    template <typename Tp, std::size_t N>
    struct ring_iterator {
    private:
      using buff_t = fixed_buffer<Tp, N>;
    };
  }
}
}

#include <archie/utils/test.h>
#include <memory>
namespace cont = archie::utils::containers;

void canCreateStackAlloc() {
  auto alloc = cont::stack_allocator<int, 4>{};
  EXPECT_EQ(4u, alloc.max_size());
}

void canAllocateWithStackAlloc() {
  auto alloc = cont::stack_allocator<int, 4>{};
  EXPECT_EQ(4u, alloc.max_size());
  auto ptr1 = alloc.allocate(2);
  EXPECT_EQ(2u, alloc.max_size());
  EXPECT_TRUE((void*)ptr1 >= (void*)&alloc);
  EXPECT_TRUE((void*)ptr1 < (void*)(&alloc + 1));

  auto ptr2 = alloc.allocate(1);
  EXPECT_EQ(1u, alloc.max_size());
  EXPECT_TRUE((void*)ptr2 >= (void*)&alloc);
  EXPECT_TRUE((void*)ptr2 < (void*)(&alloc + 1));

  EXPECT_NE(ptr1, ptr2);
}

void canDefaultConstructFixedBuffer() {
  auto buff = cont::fixed_buffer<int, 5>{};
  EXPECT_EQ(0u, buff.size());
}

void canConstructFixedBufferWithNum() {
  using buff_t = cont::fixed_buffer<int, 5>;
  EXPECT_EQ(0u, buff_t{0}.size());
  EXPECT_EQ(1u, buff_t{1}.size());

  auto buff2 = buff_t{2, 1};
  EXPECT_EQ(2u, buff2.size());
  EXPECT_EQ(1u, buff2[0]);
  EXPECT_EQ(1u, buff2[1]);
}

int main() {
  canCreateStackAlloc();
  canDefaultConstructFixedBuffer();
  canConstructFixedBufferWithNum();
  return 0;
}
