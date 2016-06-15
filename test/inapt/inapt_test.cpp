#if 0 // usage example
#include <archie/inapt.hpp>
#include <iostream>
struct error {
  enum codes : int { a = -1, b = -2, c = -3 };
  static void handle(int x) { handle(static_cast<codes>(x)); }
  static void handle(codes c) {
    switch (c) {
      case codes::a:
        std::cout << "Error A!\n";
        break;
      case codes::b:
        std::cout << "Error B!\n";
        break;
      case codes::c:
        std::cout << "Error C!\n";
        break;
      default:
        std::cout << "Unknown error!\n";
    };
  }
};

using status = archie::reserved_t<int, error::codes::a, error::codes::b, error::codes::c>;

status foo() {
  static int x = error::codes::c;
  return status(x++);
}

int main() {
  status s;
  while (!s) {
    error::handle(s.get());
    s = foo();
  }
  std::cout << "Test passed! status: " << s.get() << "\n";
  return 0;
}
#endif

#include <archie/inapt.hpp>
#include <gtest/gtest.h>
namespace
{
  using namespace archie;
  struct error {
    enum codes : int
    {
      a = -1,
      b = -2,
      c = -3
    };
  };

  using status = reserved_t<int, error::codes::a, error::codes::b, error::codes::c>;
  using failure = reserved_t<int, 0>;

  TEST(inapt, canCreateInapt)
  {
    enum
    {
      non_null = 7
    };
    status s;
    null_inapt_t null;

    {
      EXPECT_EQ(s, null);
      failure f;
      EXPECT_EQ(f, null);
    }

    {
      EXPECT_NE(s, error::codes::b);
      s = error::codes::b;
      EXPECT_EQ(s, error::codes::b);
      EXPECT_EQ(s, null);
    }

    {
      EXPECT_NE(s, non_null);
      s = non_null;
      EXPECT_EQ(s, non_null);
      EXPECT_NE(s, null);
    }

    {
      status nn{non_null};
      EXPECT_EQ(nn, non_null);
      EXPECT_NE(nn, null);
    }

    {
      s = non_null;
      EXPECT_NE(s, null);
      s = null;
      EXPECT_EQ(s, null);
    }

    {
      status a;
      EXPECT_FALSE(a);
      a = non_null;
      EXPECT_TRUE(a);
    }

    {
      status a;
      status const b(error::a);
      EXPECT_EQ(*a, error::a);
      EXPECT_EQ(*b, error::a);
    }

    {
      status a(error::b);
      status b(a);
      a = b;
      EXPECT_EQ(a, error::b);
      EXPECT_EQ(b, error::b);
    }
  }
}
