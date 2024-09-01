#include <cassert>
#include <iostream>
#include <unordered_map>

using namespace std;

int fibonacci(int n, unordered_map<int, int> &cache)
{
    auto it = cache.find(n);
    if (it != cache.end()) {
        return it->second;
    }

    int result;
    if (n == 0) {
        result = 0;
    } else if (n == 1) {
        result = 1;
    } else {
        result = fibonacci(n - 2, cache) + fibonacci(n - 1, cache);
    }

    cache[n] = result;
    return result;
}

int fibonacci(int n)
{
    static unordered_map<int, int> cache;
    return fibonacci(n, cache);
}

int main()
{
    int number_9th{34};
    assert(fibonacci(9) == number_9th);

    int number_13th{233};
    assert(fibonacci(13) == number_13th);

    int number_16th{987};
    assert(fibonacci(16) == number_16th);

    int number_20th{6765};
    assert(fibonacci(20) == number_20th);

    cout << "All tests passed!" << endl;

    return 0;
}
