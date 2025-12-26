#include "../stl/data-structure/CircularBuffer.h"

#include <gtest/gtest.h>

TEST(CircularBufferTest, ParameterizedConstructor)
{
    CircularBuffer buffer(4);

    EXPECT_EQ(buffer.capacity(), 4);
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_TRUE(buffer.isEmpty());
}

TEST(CircularBufferTest, Enqueue)
{
    CircularBuffer buffer(4);

    buffer.enqueue(1);
    buffer.enqueue(2);

    EXPECT_EQ(buffer.size(), 2);
    EXPECT_FALSE(buffer.isEmpty());
}

TEST(CircularBufferTest, Dequeue)
{
    CircularBuffer buffer(4);

    buffer.enqueue(1);
    buffer.enqueue(2);

    EXPECT_EQ(buffer.size(), 2);
    EXPECT_FALSE(buffer.isEmpty());

    int head = buffer.dequeue();
    int EXPECTED_VAL = 1;

    EXPECT_EQ(head, EXPECTED_VAL);
    EXPECT_EQ(buffer.size(), 1);
}


