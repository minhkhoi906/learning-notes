#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iostream>

using namespace std;

template <typename T> struct Vector
{
    // Default ctor
    Vector() : _data(nullptr), _size(0), _capacity(1) {}
    // Parameterized ctor
    Vector(std::initializer_list<T> vals) : _size(vals.size()), _capacity(_size * 2)
    {
        _data = new T[_capacity];
        std::copy(vals.begin(), vals.end(), _data);
    }
    // Copy ctor
    Vector(const Vector &other) : _size(other._size), _capacity(other._capacity)
    {
        _data = new T[_capacity];
        std::copy(other._data, other._data + _size, _data);
    }

    // Copy operator assignment
    Vector &operator=(const Vector &other)
    {
        if (this != &other) {
            delete[] _data;
            _size = other._size;
            _capacity = other._capacity;
            _data = new T[_capacity];
            std::copy(other._data, other._data + _size, _data);
        }
        return *this;
    }

    // Move ctor
    Vector(Vector &&other)
        : _data(other._data), _size(other._size), _capacity(other._capacity)
    {
        other._data = nullptr;
        other._size = 0;
        other._capacity = 1;
    }

    // Move operator assignment
    Vector &operator=(Vector &&other)
    {
        if (this != &other) {
            delete[] _data;
            _size = other._size;
            _capacity = other._capacity;
            _data = other._data;

            other._data = nullptr;
            other._size = 0;
            other._capacity = 1;
        }
        return *this;
    }

    // Dtor
    ~Vector() { delete[] _data; }

    bool empty() const noexcept { return _size == 0; }

    void push_back(const T &val)
    {
        if(empty()) {
            _data = new T[1];
        }

        if (_size == _capacity) {
            resever(_size * 2);
        }

        _data[_size++] = val;
    }

    void resever(const size_t size)
    {
        if (size <= _capacity) {
            return;
        }

        T *temp = new T[size];
        std::move(_data, _data + _size, temp);

        delete[] _data;
        _data = temp;
        _capacity = size;
    }

    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }

    T operator[](int idx) {
        if (idx > _size) {
            throw std::out_of_range("Index is out of range.");
        }

        return _data[idx];
    }

  private:
    T *_data;
    size_t _size;
    size_t _capacity;
};

int main()
{
    // Default ctor
    Vector<int> v1;
    // size, capacity methods
    assert(v1.size() == 0);
    assert(v1.capacity() == 1);

    // push_back method
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);

    assert(v1.size() == 3);
    assert(v1.capacity() == 4);
    // Parameterized ctor
    Vector<int> v2 = {1, 2, 3};
    assert(v2.size() == 3);
    // Copy ctor
    Vector<int> v3 = v1;
    assert(v3.size() == 3);
    // Move ctor
    Vector<int> v4 = std::move(v3);
    assert(v4.size() == 3);
    assert(v3.size() == 0);

    v3 = v4;
    assert(v3.size() == 3);
    assert(v4.size() == 3);

    v3 = std::move(v4);
    assert(v3.size() == 3);
    assert(v4.size() == 0);

    v4.push_back(1);
    assert(v4.size() == 1);
    assert(v4[0] == 1);
    v4.push_back(3);
    assert(v4.size() == 2);
    assert(v4[1] == 3);
}