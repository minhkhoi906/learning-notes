#include <chrono>
#include <future>
#include <iostream>
#include <thread>

// Function that performs some asynchronous operation
void asyncOperation(std::promise<int> prom)
{
    // Simulate work
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int result = 42; // Result of the operation

    // Set the result in the promise
    prom.set_value(result);
}

int main()
{
    // Create a std::promise object
    std::promise<int> prom;

    // Get a std::future object associated with the promise
    std::future<int> fut = prom.get_future();

    // Launch a thread to perform the asynchronous operation
    std::thread t(asyncOperation, std::move(prom));

    // Perform other work in the main thread while waiting for the result

    // Wait for the result and retrieve it
    int result = fut.get(); // This will block until the result is available

    std::cout << "Result of the asynchronous operation: " << result << std::endl;

    // Join the thread
    t.join();

    return 0;
}
