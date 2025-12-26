#include "../stl/data-structure/Array.h"

#include <gtest/gtest.h>

TEST(ArrayTest, DefaultConstructor)
{
    Array<int, 5> arr;
    EXPECT_EQ(arr.size(), 5);
}

TEST(ArrayTest, ValueInitializedConstructor)
{
    Array<int, 4> arr(42);
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 42);
    }
}
TEST(ArrayTest, CopyConstructor)
{
    Array<int, 3> a(5);
    Array<int, 3> b = a;
    for (size_t i = 0; i < b.size(); ++i) {
        EXPECT_EQ(b[i], 5);
    }
}
TEST(ArrayTest, CopyAssignment)
{
    Array<int, 2> a(7);
    Array<int, 2> b;
    b = a;
    for (size_t i = 0; i < b.size(); ++i) {
        EXPECT_EQ(b[i], 7);
    }
}
TEST(ArrayTest, MoveConstructor)
{
    Array<int, 3> a(9);
    Array<int, 3> b = std::move(a);
    for (size_t i = 0; i < b.size(); ++i) {
        EXPECT_EQ(b[i], 9);
    }
}
TEST(ArrayTest, MoveAssignment)
{
    Array<int, 4> a(11);
    Array<int, 4> b;
    b = std::move(a);
    for (size_t i = 0; i < b.size(); ++i) {
        EXPECT_EQ(b[i], 11);
    }
}
TEST(ArrayTest, ElementAccess)
{
    Array<char, 3> arr('x');
    arr[0] = 'a';
    arr[1] = 'b';
    arr[2] = 'c';
    EXPECT_EQ(arr[0], 'a');
    EXPECT_EQ(arr[1], 'b');
    EXPECT_EQ(arr[2], 'c');
}
TEST(ArrayTest, ConstAccess)
{
    const Array<char, 2> arr('z');
    EXPECT_EQ(arr[0], 'z');
    EXPECT_EQ(arr[1], 'z');
}
TEST(ArrayTest, Size)
{
    Array<int, 10> arr;
    EXPECT_EQ(arr.size(), 10);
}
TEST(ArrayTest, OutOfRange)
{
    Array<int, 2> arr(1);
    EXPECT_THROW(arr[3], std::out_of_range); // Should not reach here
}
