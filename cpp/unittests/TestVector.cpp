#include "../stl/data-structure/Vector.h"

#include <gtest/gtest.h>

struct TestObject {
    int value;
    explicit TestObject(int val = 0) : value(val) {}
};

// Define a base test fixture
class VectorTest : public ::testing::Test
{
  protected:
    static constexpr uint32_t EXPECTED_SIZE = 100;
};

// ✅ **Test Default Constructor**
TEST_F(VectorTest, DefaultConstructor)
{
    Vector<TestObject> vector;

    EXPECT_TRUE(vector.empty());
    EXPECT_EQ(vector.size(), 0);
    EXPECT_EQ(vector.capacity(), 0); // Default capacity is initialized to 1
}

// ✅ **Test Parameterized Constructor**
TEST_F(VectorTest, ParameterizedConstructor)
{
    Vector<TestObject> vector(EXPECTED_SIZE);

    EXPECT_FALSE(vector.empty());
    EXPECT_EQ(vector.size(), EXPECTED_SIZE);
    EXPECT_EQ(vector.capacity(), EXPECTED_SIZE);

    // Ensure all elements are default initialized
    for (size_t i = 0; i < vector.size(); ++i) {
        EXPECT_EQ(vector[i].value, 0);
    }
}

// ✅ **Test Copy Constructor**
TEST_F(VectorTest, CopyConstructor)
{
    Vector<TestObject> first_vector(EXPECTED_SIZE);

    // Initialize with specific values
    for (size_t i = 0; i < EXPECTED_SIZE; ++i) {
        first_vector[i] = TestObject(i);
    }

    Vector<TestObject> second_vector = first_vector;

    // Validate both vectors
    EXPECT_EQ(first_vector.size(), EXPECTED_SIZE);
    EXPECT_EQ(second_vector.size(), EXPECTED_SIZE);
    EXPECT_EQ(first_vector.capacity(), EXPECTED_SIZE);
    EXPECT_EQ(second_vector.capacity(), EXPECTED_SIZE);

    // Ensure deep copy (values match)
    for (size_t i = 0; i < EXPECTED_SIZE; ++i) {
        EXPECT_EQ(second_vector[i].value, first_vector[i].value);
    }
}

// ✅ **Test Copy Assignment**
TEST_F(VectorTest, CopyAssignment)
{
    Vector<TestObject> first_vector(EXPECTED_SIZE);

    for (size_t i = 0; i < EXPECTED_SIZE; ++i) {
        first_vector[i] = TestObject(i);
    }

    Vector<TestObject> second_vector;
    second_vector = first_vector;

    EXPECT_EQ(second_vector.size(), EXPECTED_SIZE);
    EXPECT_EQ(second_vector.capacity(), EXPECTED_SIZE);

    for (size_t i = 0; i < EXPECTED_SIZE; ++i) {
        EXPECT_EQ(second_vector[i].value, first_vector[i].value);
    }
}

// ✅ **Test Move Constructor**
TEST_F(VectorTest, MoveConstructor)
{
    Vector<TestObject> first_vector(EXPECTED_SIZE);

    for (size_t i = 0; i < EXPECTED_SIZE; ++i) {
        first_vector[i] = TestObject(i);
    }

    Vector<TestObject> second_vector = std::move(first_vector);

    EXPECT_EQ(second_vector.size(), EXPECTED_SIZE);
    EXPECT_EQ(second_vector.capacity(), EXPECTED_SIZE);

    // Ensure moved-from vector is reset
    EXPECT_EQ(first_vector.size(), 0);
    EXPECT_EQ(first_vector.capacity(), 0);
}

// ✅ **Test Move Assignment**
TEST_F(VectorTest, MoveAssignment)
{
    Vector<TestObject> first_vector(EXPECTED_SIZE);
    Vector<TestObject> second_vector;

    for (size_t i = 0; i < EXPECTED_SIZE; ++i) {
        first_vector[i] = TestObject(i);
    }

    second_vector = std::move(first_vector);

    EXPECT_EQ(second_vector.size(), EXPECTED_SIZE);
    EXPECT_EQ(second_vector.capacity(), EXPECTED_SIZE);

    // Ensure moved-from vector is reset
    EXPECT_EQ(first_vector.size(), 0);
    EXPECT_EQ(first_vector.capacity(), 0);
}

// ✅ **Test `reserve()` Method**
TEST_F(VectorTest, Reserve)
{
    Vector<TestObject> vector(10);

    vector.reserve(50);
    EXPECT_EQ(vector.capacity(), 50);
    EXPECT_EQ(vector.size(), 10);

    // Reserving a smaller size should not reduce capacity
    vector.reserve(5);
    EXPECT_EQ(vector.capacity(), 50);
}

// ✅ **Test `push_back()` Method**
TEST_F(VectorTest, PushBack)
{
    Vector<TestObject> vector;

    for (int i = 0; i < 10; ++i) {
        vector.push_back(TestObject(i));
    }

    EXPECT_EQ(vector.size(), 10);
    EXPECT_GE(vector.capacity(), 10); // Capacity should grow dynamically

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(vector[i].value, i);
    }
}

// ✅ **Test `operator[]` Bounds Checking**
TEST_F(VectorTest, OutOfBoundsAccess)
{
    Vector<TestObject> vector(5);

    // Should not throw for valid index
    EXPECT_NO_THROW(vector[0]);

    // Should throw for out-of-bounds access
    EXPECT_THROW(vector[5], std::out_of_range);
}

// ✅ **Test `empty()` Method**
TEST_F(VectorTest, Empty)
{
    Vector<TestObject> vector;
    EXPECT_TRUE(vector.empty());

    vector.push_back(TestObject(1));
    EXPECT_FALSE(vector.empty());
}

// ✅ **Test Self-Assignment (Copy)**
TEST_F(VectorTest, SelfAssignmentCopy)
{
    Vector<TestObject> vector(EXPECTED_SIZE);

    for (size_t i = 0; i < EXPECTED_SIZE; ++i) {
        vector[i] = TestObject(i);
    }

    vector = vector; // Self-assignment

    EXPECT_EQ(vector.size(), EXPECTED_SIZE);
    EXPECT_EQ(vector.capacity(), EXPECTED_SIZE);

    for (size_t i = 0; i < EXPECTED_SIZE; ++i) {
        EXPECT_EQ(vector[i].value, i);
    }
}

// ✅ **Test Self-Assignment (Move)**
TEST_F(VectorTest, SelfAssignmentMove)
{
    Vector<TestObject> vector(EXPECTED_SIZE);

    vector = std::move(vector); // Self-move

    EXPECT_EQ(vector.size(), EXPECTED_SIZE);
    EXPECT_EQ(vector.capacity(), EXPECTED_SIZE);
}

// ✅ **Test Destructor (Memory Leak Check)**
TEST_F(VectorTest, Destructor)
{
    Vector<TestObject> *vector = new Vector<TestObject>(EXPECTED_SIZE);
    delete vector; // Should not crash or leak memory
}
