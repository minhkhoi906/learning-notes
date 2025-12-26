#include "../stl/data-structure/QueueSafe.h"

#include <atomic>
#include <gtest/gtest.h>
#include <thread>
#include <vector>

TEST(QueueSafeTest, SinglePushPop)
{
    QueueSafe<int> q;
    q.push(42);
    EXPECT_EQ(q.pop(), 42);
}

TEST(QueueSafeTest, MultiplePushPop)
{
    QueueSafe<std::string> q;
    q.push("first");
    q.push("second");

    EXPECT_EQ(q.pop(), "first");
    EXPECT_EQ(q.pop(), "second");
}

TEST(QueueSafeTest, BlockingPopWaitsUntilPush)
{
    QueueSafe<int> q;
    std::atomic<bool> pop_started = false;
    std::atomic<bool> push_done = false;

    std::thread pop_thread([&]() {
        pop_started = true;
        EXPECT_EQ(q.pop(), 99);
        push_done = true;
    });

    // Wait to ensure pop is waiting
    while (!pop_started)
        std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    q.push(99);

    pop_thread.join();
    EXPECT_TRUE(push_done);
}

TEST(QueueSafeTest, MultipleThreadedPushPop)
{
    QueueSafe<int> q;
    const int count = 1000;
    std::vector<int> results;
    std::mutex results_mutex;

    std::thread producer([&]() {
        for (int i = 0; i < count; ++i) {
            q.push(i);
        }
    });

    std::thread consumer([&]() {
        for (int i = 0; i < count; ++i) {
            int val = q.pop();
            std::lock_guard<std::mutex> lock(results_mutex);
            results.push_back(val);
        }
    });

    producer.join();
    consumer.join();

    EXPECT_EQ(results.size(), count);
    std::sort(results.begin(), results.end());
    for (int i = 0; i < count; ++i) {
        EXPECT_EQ(results[i], i);
    }
}
