#include <iostream>
#include <mutex>
#include <string_view>
#include <thread>
#include <atomic>
#include <vector>

std::atomic_int g_i(0);
std::vector<int> v_i;
// volatile int g_i = 0;

std::mutex g_i_mutex; // protects g_i

void safe_increment(int iterations)
{
    // const std::lock_guard<std::mutex> lock(g_i_mutex);
    while (iterations-- > 0) {
        const std::lock_guard<std::mutex> lock(g_i_mutex);
        g_i = g_i + 1;
    }
    std::cout << "thread #" << std::this_thread::get_id() << ", g_i: " << g_i
              << '\n';

    // g_i_mutex is automatically released when lock goes out of scope
}

void unsafe_increment(int iterations)
{
    while (iterations-- > 0) {
        const std::lock_guard<std::mutex> lock(g_i_mutex);
        v_i.push_back(1);
    }

    std::cout << "thread #" << std::this_thread::get_id() << ", g_i: " << g_i
              << '\n';
}

int main()
{
    auto test = [](std::string_view fun_name, auto fun) {
        v_i.clear();
        std::cout << fun_name << ":\nbefore, g_i: " << v_i.size() << '\n';
        {
            std::thread t1(fun, 1000000);
            std::thread t2(fun, 1000000);

            t1.join();
            t2.join();
        }
        std::cout << "after, g_i: " << v_i.size() << "\n\n";
    };

    // test("safe_increment", safe_increment);
    test("unsafe_increment", unsafe_increment);
}