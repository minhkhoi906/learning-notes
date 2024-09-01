#include <atomic>
#include <future>
#include <iostream>
#include <mutex>
// #include <thread>

using namespace std;

class SafeSingleton
{
  public:
    static SafeSingleton &getInstance()
    {
        static SafeSingleton instance;
        return instance;
    }

    void increase()
    {
        lock_guard<mutex> guard(_lock);

        ++_count;
    }
    void display(const int id)
    {
        lock_guard<mutex> guard(_lock);
        cout << "Thread " << id << ": " << _count << endl;
    }

  private:
    SafeSingleton() : _count(0) {}
    ~SafeSingleton() {}

    SafeSingleton(const SafeSingleton &) = delete;
    SafeSingleton &operator=(const SafeSingleton &) = delete;
    int _count;
    mutex _lock;
};

void client_code(const int id)
{
    for (size_t i = 0; i < 1000; ++i) {
        SafeSingleton::getInstance().increase();
    }
    SafeSingleton::getInstance().display(id);
}

int main()
{
    auto f1 = std::async(std::launch::async, client_code, 1);
    auto f2 = std::async(std::launch::async, client_code, 2);

    return 0;
}
