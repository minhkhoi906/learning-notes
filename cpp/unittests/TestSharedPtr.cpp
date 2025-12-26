#include "../stl/smart-pointers/SharedPtr.h"

#include <gtest/gtest.h>

struct TestObject
{
    int value;
    explicit TestObject(int val) : value(val) {}
};

TEST(SharedPtrTest, ConstructorAndDereference)
{
    SharedPtr<TestObject> ptr(new TestObject(42));
    ASSERT_TRUE(ptr);
    EXPECT_EQ(ptr->value, 42);
    EXPECT_EQ((*ptr).value, 42);
}
TEST(SharedPtrTest, CopyConstructor)
{
    SharedPtr<TestObject> ptr1(new TestObject(100));
    EXPECT_EQ(ptr1.use_count(), 1);
    SharedPtr<TestObject> ptr2(ptr1); // Copy constructor

    EXPECT_TRUE(ptr1);
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr1->value, 100);
    EXPECT_EQ(ptr2->value, 100);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}
TEST(SharedPtrTest, MoveConstructor)
{
    SharedPtr<TestObject> ptr1(new TestObject(200));
    SharedPtr<TestObject> ptr2(std::move(ptr1)); // Move constructor

    EXPECT_FALSE(ptr1); // ptr1 should be null after move
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr2->value, 200);
}
TEST(SharedPtrTest, CopyAssignment)
{
    SharedPtr<TestObject> ptr1(new TestObject(300));
    SharedPtr<TestObject> ptr2;
    ptr2 = ptr1; // Copy assignment

    EXPECT_TRUE(ptr1);
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr1->value, 300);
    EXPECT_EQ(ptr2->value, 300);
}
TEST(SharedPtrTest, MoveAssignment)
{
    SharedPtr<TestObject> ptr1(new TestObject(400));
    SharedPtr<TestObject> ptr2;
    ptr2 = std::move(ptr1); // Move assignment

    EXPECT_FALSE(ptr1); // ptr1 should be null after move
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr2->value, 400);
}
TEST(SharedPtrTest, Reset)
{
    SharedPtr<TestObject> ptr(new TestObject(10));
    ptr.reset(new TestObject(20));

    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr->value, 20);

    ptr.reset();
    EXPECT_FALSE(ptr); // Should be nullptr after reset
}
TEST(SharedPtrTest, UseCount)
{
    SharedPtr<TestObject> ptr1(new TestObject(50));
    EXPECT_EQ(ptr1.use_count(), 1);

    {
        SharedPtr<TestObject> ptr2(ptr1); // Copy constructor
        EXPECT_EQ(ptr1.use_count(), 2);
        EXPECT_EQ(ptr2.use_count(), 2);
    } // ptr2 goes out of scope

    EXPECT_EQ(ptr1.use_count(), 1); // ptr1 should still be valid
}
TEST(SharedPtrTest, EqualityOperators)
{
    SharedPtr<TestObject> ptr1(new TestObject(10));
    SharedPtr<TestObject> ptr2(new TestObject(10));
    SharedPtr<TestObject> ptr3(ptr1);

    EXPECT_TRUE(ptr1 == ptr3); // Same object
    EXPECT_FALSE(ptr1 == ptr2); // Different objects
    EXPECT_TRUE(ptr1 != ptr2);  // Different objects
    EXPECT_FALSE(ptr1 != ptr3); // Same object
}
TEST(SharedPtrTest, BoolConversion)
{
    SharedPtr<TestObject> ptr1(new TestObject(100));
    SharedPtr<TestObject> ptr2;

    EXPECT_TRUE(ptr1);  // Should be true
    EXPECT_FALSE(ptr2); // Should be false
}
TEST(SharedPtrTest, Get)
{
    SharedPtr<TestObject> ptr(new TestObject(60));
    EXPECT_EQ(ptr.get()->value, 60);
}
// TEST(SharedPtrTest, CustomDeleter)
// {
//     bool deleted = false;
//     auto deleter = [&deleted](TestObject *ptr) {
//         delete ptr;
//         deleted = true;
//     };

//     {
//         SharedPtr<TestObject> ptr(new TestObject(70), deleter);
//         EXPECT_TRUE(ptr);
//         EXPECT_EQ(ptr->value, 70);
//     } // ptr goes out of scope

//     EXPECT_TRUE(deleted); // Deleter should have been called
// }
// TEST(SharedPtrTest, CustomDeleterWithNullptr)
// {
//     bool deleted = false;
//     auto deleter = [&deleted](TestObject *ptr) {
//         if (ptr) {
//             delete ptr;
//             deleted = true;
//         }
//     };

//     SharedPtr<TestObject> ptr(nullptr, deleter);
//     EXPECT_FALSE(ptr); // Should be nullptr
//     EXPECT_FALSE(deleted); // Deleter should not be called
// }
// TEST(SharedPtrTest, CustomDeleterWithNonNullptr)
// {
//     bool deleted = false;
//     auto deleter = [&deleted](TestObject *ptr) {
//         if (ptr) {
//             delete ptr;
//             deleted = true;
//         }
//     };

//     {
//         SharedPtr<TestObject> ptr(new TestObject(80), deleter);
//         EXPECT_TRUE(ptr);
//         EXPECT_EQ(ptr->value, 80);
//     } // ptr goes out of scope

//     EXPECT_TRUE(deleted); // Deleter should have been called
// }
// TEST(SharedPtrTest, CustomDeleterWithMove)
// {
//     bool deleted = false;
//     auto deleter = [&deleted](TestObject *ptr) {
//         delete ptr;
//         deleted = true;
//     };

//     SharedPtr<TestObject> ptr1(new TestObject(90), deleter);
//     SharedPtr<TestObject> ptr2(std::move(ptr1)); // Move constructor

//     EXPECT_FALSE(ptr1); // ptr1 should be null after move
//     EXPECT_TRUE(ptr2);
//     EXPECT_EQ(ptr2->value, 90);
//     EXPECT_TRUE(deleted); // Deleter should have been called
// }
// TEST(SharedPtrTest, CustomDeleterWithCopy)
// {
//     bool deleted = false;
//     auto deleter = [&deleted](TestObject *ptr) {
//         delete ptr;
//         deleted = true;
//     };

//     SharedPtr<TestObject> ptr1(new TestObject(100), deleter);
//     SharedPtr<TestObject> ptr2(ptr1); // Copy constructor

//     EXPECT_TRUE(ptr1);
//     EXPECT_TRUE(ptr2);
//     EXPECT_EQ(ptr1->value, 100);
//     EXPECT_EQ(ptr2->value, 100);
//     EXPECT_EQ(ptr1.use_count(), 2); // Use count should be 2
// }
// TEST(SharedPtrTest, CustomDeleterWithReset)
// {
//     bool deleted = false;
//     auto deleter = [&deleted](TestObject *ptr) {
//         delete ptr;
//         deleted = true;
//     };

//     SharedPtr<TestObject> ptr(new TestObject(110), deleter);
//     EXPECT_TRUE(ptr);
//     EXPECT_EQ(ptr->value, 110);

//     ptr.reset(new TestObject(120)); // Reset with new object
//     EXPECT_TRUE(ptr);
//     EXPECT_EQ(ptr->value, 120);
//     EXPECT_TRUE(deleted); // Deleter should have been called for the old object
// }
// TEST(SharedPtrTest, CustomDeleterWithResetToNullptr)
// {
//     bool deleted = false;
//     auto deleter = [&deleted](TestObject *ptr) {
//         delete ptr;
//         deleted = true;
//     };

//     SharedPtr<TestObject> ptr(new TestObject(130), deleter);
//     EXPECT_TRUE(ptr);
//     EXPECT_EQ(ptr->value, 130);

//     ptr.reset(); // Reset to nullptr
//     EXPECT_FALSE(ptr); // Should be nullptr after reset
//     EXPECT_TRUE(deleted); // Deleter should have been called for the old object
// }
// TEST(SharedPtrTest, CustomDeleterWithRelease)
// {
//     bool deleted = false;
//     auto deleter = [&deleted](TestObject *ptr) {
//         delete ptr;
//         deleted = true;
//     };

//     SharedPtr<TestObject> ptr(new TestObject(140), deleter);
//     TestObject *rawPtr = ptr.release();

//     EXPECT_FALSE(ptr); // SharedPtr should be empty after release
//     EXPECT_NE(rawPtr, nullptr);
//     EXPECT_EQ(rawPtr->value, 140);
//     delete rawPtr; // Clean up the raw pointer
//     EXPECT_TRUE(deleted); // Deleter should have been called
// }
