#include <cstddef>
#include <utility>
#include <new>

namespace archie {
namespace utils {
  namespace containers {

    template <typename Tp, std::size_t N>
    struct fixed_buffer {
      using value_type = Tp;
      using pointer = Tp*;
      using reference = Tp&;
      using const_reference = Tp const&;
      using const_pointer = Tp const*;
      using size_type = std::size_t;
      using iterator = pointer;
      using const_iterator = const_pointer;

      explicit fixed_buffer(size_type n) {
        while (n--) emplace_back();
      }

      template <typename Up>
      fixed_buffer(size_type n, Up const& ref) {
        while (n--) emplace_back(ref);
      }

      fixed_buffer() : fixed_buffer(0u) {}

      const_reference operator[](size_type n) const { return store.array[n]; }
      const_reference front() const { return operator[](0); }
      const_iterator begin() const { return &front(); }
      const_iterator end() const { return begin() + size(); }

      reference operator[](size_type n) { return store.array[n]; }
      reference front() { return operator[](0); }
      iterator begin() { return &front(); }
      iterator end() { return begin() + size(); }

      size_type size() const { return size_; }

      template <typename... Us>
      void emplace_back(Us&&... us) {
        new (end()) value_type(std::forward<Us>(us)...);
        ++size_;
      }

    private:
      union storage {
        char raw[N * sizeof(value_type)];
        value_type array[N];
      };
      storage store;
      size_type size_ = 0;
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

namespace cont = archie::utils::containers;

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
  canDefaultConstructFixedBuffer();
  canConstructFixedBufferWithNum();
  return 0;
}
