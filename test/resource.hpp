#pragma once
#include <utility>

namespace archie {
namespace test {
  struct resource {
    using value_type = int;
    static int get_id() {
      static int id = 0;
      return id++;
    };
    explicit resource(value_type i) : ptr(new value_type(i)), id_(get_id()) {}
    resource() : resource(0) {}
    resource(resource const& r) : resource(*(r.ptr)) {}
    resource(resource&& r) : ptr(r.ptr), id_(get_id()) { r.ptr = nullptr; }
    resource& operator=(resource const& r) {
      *ptr = *r.ptr;
      return *this;
    }
    resource& operator=(resource&& r) {
      using std::swap;
      swap(ptr, r.ptr);
      return *this;
    }
    ~resource() {
      if (ptr) delete ptr;
    }
    explicit operator bool() const { return ptr != nullptr; }
    operator value_type() const { return *ptr; }
    int id() const { return id_; }
    value_type value() const { return *ptr; }
    bool operator==(resource const& r) const { return *ptr == *r.ptr; }
    bool operator!=(resource const& r) const { return !(*this == r); }

  private:
    value_type* ptr = nullptr;
    int id_ = 0;
  };
}
}
