#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>

using namespace std;

template <typename T, size_t N> class Array
{
  public:
    Array() : _data{} {}

    Array(const T &default_val)
    {
        for (auto &item : _data) {
            item = default_val;
        }
    }

    Array(std::initializer_list<T> list)
    {
        if (list.size() > N) {
            throw std::out_of_range("Initializer list size exceeds array size");
        }
        std::copy_n(list.begin(), list.size(), _data);
    }

    size_t size() const { return N; }

    T &operator[](size_t idx)
    {
        if (idx >= N) {
            throw std::out_of_range("Index " + std::to_string(idx) +
                                    " is out of range");
        }
        return _data[idx];
    }

    const T &operator[](size_t idx) const
    {
        if (idx >= N) {
            throw std::out_of_range("Index " + std::to_string(idx) +
                                    " is out of range");
        }
        return _data[idx];
    }

  private:
    T _data[N];
};

int main()
{
    // Default ctor
    Array<int, 2> arr1;
    Array<float, 3> arr2;

    assert(arr1.size() == 2);
    assert(arr2.size() == 3);
    assert(arr1[0] == 0);
    assert(arr2[0] == 0.0f);

    // Parameterized ctor with default value
    Array<int, 3> arr3(2);
    assert(arr3.size() == 3);
    assert(arr3[0] == 2);

    // Access elements and check bounds
    try {
        arr3[3] = 5; // This should throw an exception
    } catch (const std::out_of_range &e) {
        assert(true); // Expected to catch the out_of_range exception
    }

    Array<int, 3> arr4 = {1, 3, 5};
    assert(arr4.size() == 3);
    assert(arr4[0] == 1);
    assert(arr4[1] == 3);
    assert(arr4[2] == 5);

    return 0;
}
