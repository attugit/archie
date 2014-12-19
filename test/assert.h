#include <cassert>

#define ASSERT_TRUE(x) assert((x) == true);
#define ASSERT_FALSE(x) assert((x) == false);
#define ASSERT_EQ(x, y) assert((x) == (y));
#define ASSERT_NE(x, y) assert((x) != (y));
#define ASSERT_LE(x, y) assert((x) <= (y));

#define EXPECT_EQ(x, y) ASSERT_EQ(x, y)
#define EXPECT_TRUE(x) ASSERT_TRUE(x)
#define EXPECT_FALSE(x) ASSERT_FALSE(x)
#define EXPECT_LE(x, y) ASSERT_LE(x, y)
#define EXPECT_NE(x, y) ASSERT_NE(x, y)

