#include <cassert>

#define EXPECT_EQ(x, y) assert(x == y);
#define EXPECT_TRUE(x) assert(x == true);
#define EXPECT_FALSE(x) assert(x == false);
#define EXPECT_LE(x, y) assert(x <= y);

#define ASSERT_EQ(x, y) EXPECT_EQ(x, y)

#define TEST_F(x, y) void y()
