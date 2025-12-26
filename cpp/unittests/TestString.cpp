#include "../stl/data-types/String.h"

#include <gtest/gtest.h>

TEST(StringTest, ConstructorAndCStr)
{
    String s("hello");
    EXPECT_STREQ(s.c_str(), "hello");
    EXPECT_EQ(s.size(), 5);
}

TEST(StringTest, CopyConstructor)
{
    String s1("world");
    String s2(s1);
    EXPECT_STREQ(s2.c_str(), "world");
    EXPECT_EQ(s2.size(), 5);
}

TEST(StringTest, MoveConstructor)
{
    String s1("move");
    String s2(std::move(s1));
    EXPECT_STREQ(s2.c_str(), "move");
    EXPECT_EQ(s2.size(), 4);
    EXPECT_EQ(s1.c_str(), nullptr);
}

TEST(StringTest, CopyAssignment)
{
    String s1("copy");
    String s2;
    s2 = s1;
    EXPECT_STREQ(s2.c_str(), "copy");
    EXPECT_EQ(s2.size(), 4);
}

TEST(StringTest, MoveAssignment)
{
    String s1("move assign");
    String s2;
    s2 = std::move(s1);
    EXPECT_STREQ(s2.c_str(), "move assign");
    EXPECT_EQ(s2.size(), 11);
    EXPECT_EQ(s1.c_str(), nullptr);
}

TEST(StringTest, SubscriptOperator)
{
    String s("abc");
    EXPECT_EQ(s[0], 'a');
    EXPECT_EQ(s[1], 'b');
    EXPECT_EQ(s[2], 'c');
    EXPECT_THROW(s[3], std::out_of_range);
}
