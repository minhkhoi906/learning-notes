#include "../stl/UniquePtr.h"

#include <gtest/gtest.h>

struct TestObject {
    int value;
    explicit TestObject(int val) : value(val) {}
};

// Test constructor and operator*
TEST(UniquePtrTest, ConstructorAndDereference)
{
    UniquePtr<TestObject> ptr(new TestObject(42));
    ASSERT_TRUE(ptr);
    EXPECT_EQ(ptr->value, 42);
    EXPECT_EQ((*ptr).value, 42);
}

// Test move constructor
TEST(UniquePtrTest, MoveConstructor)
{
    UniquePtr<TestObject> ptr1(new TestObject(100));
    UniquePtr<TestObject> ptr2(std::move(ptr1));

    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr2->value, 100);
    EXPECT_FALSE(ptr1); // ptr1 should be null after move
}

// Test move assignment
TEST(UniquePtrTest, MoveAssignment)
{
    UniquePtr<TestObject> ptr1(new TestObject(200));
    UniquePtr<TestObject> ptr2;

    ptr2 = std::move(ptr1);
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr2->value, 200);
    EXPECT_FALSE(ptr1);
}

// Test reset method
TEST(UniquePtrTest, Reset)
{
    UniquePtr<TestObject> ptr(new TestObject(10));
    ptr.reset(new TestObject(20));

    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr->value, 20);

    ptr.reset();
    EXPECT_FALSE(ptr); // Should be nullptr after reset
}

// Test release method
TEST(UniquePtrTest, Release)
{
    UniquePtr<TestObject> ptr(new TestObject(30));
    TestObject *rawPtr = ptr.release();

    EXPECT_FALSE(ptr); // UniquePtr should be empty after release
    EXPECT_NE(rawPtr, nullptr);
    EXPECT_EQ(rawPtr->value, 30);

    delete rawPtr; // Clean up to prevent memory leak
}

// Test get method
TEST(UniquePtrTest, Get)
{
    UniquePtr<TestObject> ptr(new TestObject(40));
    EXPECT_EQ(ptr.get()->value, 40);
}

// Test delete on destruction
struct MockDeleter {
    static bool deleted;
    ~MockDeleter() { deleted = true; }
};

bool MockDeleter::deleted = false;

TEST(UniquePtrTest, Destructor)
{
    {
        UniquePtr<MockDeleter> ptr(new MockDeleter);
    }
    EXPECT_TRUE(MockDeleter::deleted); // Ensure the object was deleted
}
