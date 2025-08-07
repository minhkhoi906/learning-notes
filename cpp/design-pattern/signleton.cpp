#include <iostream>
#include <mutex>
#include <thread>

// Define a modern, thread-safe singleton
class LazySingleton
{
  private:
    // The constructor is private to prevent direct instantiation.
    // Making it private forces the use of getInstance().
    LazySingleton() { std::cout << "Singleton instance created." << std::endl; }

    // The destructor is also private. The singleton instance is managed
    // by the C++ runtime and will be automatically destroyed at program exit.
    // This prevents manual deletion and potential issues.
    ~LazySingleton() { std::cout << "Singleton instance destroyed." << std::endl; }

  public:
    // Prevent copying and assignment to ensure there is only one instance.
    // These methods are "deleted" in modern C++.
    LazySingleton(const LazySingleton &) = delete;
    LazySingleton &operator=(const LazySingleton &) = delete;
    LazySingleton(LazySingleton &&) = delete;
    LazySingleton &operator=(LazySingleton &&) = delete;

    // This is the thread-safe way to get the singleton instance.
    // The static local variable 'instance' is guaranteed to be initialized
    // only once, even in a multi-threaded environment (as of C++11).
    static LazySingleton &getInstance()
    {
        static LazySingleton instance;
        return instance;
    }

    // Example member function to demonstrate usage
    void doSomething()
    {
        std::cout << "Singleton is doing something..." << std::endl;
    }
};

// Main function to demonstrate the singleton
int main()
{
    // Get the instance and use it
    std::cout << "Attempting to get instance 1..." << std::endl;
    LazySingleton &s1 = LazySingleton::getInstance();
    s1.doSomething();

    // Get the instance again. It should be the same object.
    std::cout << "Attempting to get instance 2..." << std::endl;
    LazySingleton &s2 = LazySingleton::getInstance();
    s2.doSomething();

    // The two references should point to the same object
    if (&s1 == &s2) {
        std::cout << "s1 and s2 point to the same object." << std::endl;
    }

    // Uncommenting the following lines would result in a compile-time error
    // due to the deleted copy constructor and assignment operator:
    // LazySingleton s3 = s1; // Error: use of deleted function
    // LazySingleton s4;      // Error: 'LazySingleton::LazySingleton()' is private

    return 0;
}
