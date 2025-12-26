#include "../stl/data-structure/Queue.h"

#include <gtest/gtest.h>
#include <string>

// Test fixture
class QueueTest : public ::testing::Test
{
  protected:
    Queue<int> int_queue;
    Queue<std::string> str_queue;
};

TEST_F(QueueTest, InitiallyEmpty)
{
    EXPECT_TRUE(int_queue.empty());
    EXPECT_TRUE(str_queue.empty());
}

TEST_F(QueueTest, PushMakesQueueNonEmpty)
{
    int_queue.push(42);
    EXPECT_FALSE(int_queue.empty());

    str_queue.push("hello");
    EXPECT_FALSE(str_queue.empty());
}

TEST_F(QueueTest, PopReturnsToEmpty)
{
    int_queue.push(1);
    int_queue.pop();
    EXPECT_TRUE(int_queue.empty());
}

TEST_F(QueueTest, MultiplePushesAndPops)
{
    int_queue.push(10);
    int_queue.push(20);
    int_queue.push(30);

    int_queue.pop();
    int_queue.pop();

    EXPECT_FALSE(int_queue.empty());
    int_queue.pop();
    EXPECT_TRUE(int_queue.empty());
}

TEST_F(QueueTest, CopyConstructorCreatesIdenticalQueue)
{
    int_queue.push(1);
    int_queue.push(2);
    int_queue.push(3);

    Queue<int> copy(int_queue);
    EXPECT_FALSE(copy.empty());
}

TEST_F(QueueTest, CopyAssignmentCreatesIdenticalQueue)
{
    int_queue.push(100);
    int_queue.push(200);

    Queue<int> other;
    other = int_queue;

    EXPECT_FALSE(other.empty());
}

TEST_F(QueueTest, MoveConstructorTransfersOwnership)
{
    int_queue.push(5);
    int_queue.push(6);

    Queue<int> moved(std::move(int_queue));
    EXPECT_TRUE(int_queue.empty());
    EXPECT_FALSE(moved.empty());
}

TEST_F(QueueTest, MoveAssignmentTransfersOwnership)
{
    int_queue.push(7);
    int_queue.push(8);

    Queue<int> other;
    other = std::move(int_queue);
    EXPECT_TRUE(int_queue.empty());
    EXPECT_FALSE(other.empty());
}
