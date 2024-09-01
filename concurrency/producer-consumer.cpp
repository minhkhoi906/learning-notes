#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::queue<int> buffer;                // Shared buffer
const unsigned int maxBufferSize = 10; // Maximum buffer size

std::mutex mtx;                      // Mutex for critical section
std::condition_variable cv_producer; // Condition variable for producer
std::condition_variable cv_consumer; // Condition variable for consumer

bool done = false; // Flag to stop the process

void producer(int id)
{
    int data = 0;
    while (!done) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_producer.wait(lock, [] { return buffer.size() < maxBufferSize; });

        buffer.push(data);
        std::cout << "Producer " << id << " produced " << data++ << std::endl;

        lock.unlock();
        cv_consumer.notify_all(); // Notify consumer that data is available
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    }
}

void consumer(int id)
{
    while (!done) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_consumer.wait(lock, [] { return !buffer.empty(); });

        int data = buffer.front();
        buffer.pop();
        std::cout << "Consumer " << id << " consumed " << data << std::endl;

        lock.unlock();
        cv_producer.notify_all(); // Notify producer that space is available
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Simulate work
    }
}

int main()
{
    std::thread p1(producer, 1);
    std::thread p2(producer, 2);
    std::thread c1(consumer, 1);
    std::thread c2(consumer, 2);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    done = true; // Stop the threads after some time

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    std::cout << "Done!" << std::endl;
    return 0;
}
