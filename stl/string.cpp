#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

class String
{
  private:
    char *data_;
    size_t size_;

  public:
    // Default constructor
    String() : data_(nullptr), size_(0) {}

    // Parameterized constructor
    String(const char *str) : data_(nullptr), size_(0)
    {
        if (str) {
            size_ = strlen(str);
            data_ = new char[size_ + 1];
            strcpy(data_, str);
        }
    }

    // Copy constructor
    String(const String &other) : data_(nullptr), size_(0)
    {
        if (other.size_) {
            size_ = other.size_;
            data_ = new char[size_ + 1];
            strcpy(data_, other.data_);
        }
    }

    // Copy assignment
    String &operator=(const String &other)
    {
        if (this != &other) {
            char *new_data = nullptr;
            if (other.size_) {
                new_data = new char[other.size_ + 1];
                strcpy(new_data, other.data_);
            }
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
        }
        return *this;
    }

    // Move constructor
    String(String &&other) noexcept : data_(nullptr), size_(0)
    {
        swap(data_, other.data_);
        swap(size_, other.size_);
    }

    // Move assignment
    String &operator=(String &&other) noexcept
    {
        if (this != &other) {
            swap(data_, other.data_);
            swap(size_, other.size_);
        }
        return *this;
    }

    const char *c_str() const { return data_; }

    size_t size() const { return size_; }

    char operator[](size_t idx) const
    {
        if (idx >= size_) {
            throw std::out_of_range("Index is out of range.");
        }
        return data_[idx];
    }

    ~String() { delete[] data_; }
};

ostream &operator<<(ostream &out, const String &str)
{
    if (str.c_str() != nullptr) {
        out << str.c_str();
    }
    return out;
}

void display(const String &str) { cout << str << endl; }

struct AllocationMetrics {
    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;

    uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size) {
    s_AllocationMetrics.TotalAllocated += size;

    return malloc(size);
}

void operator delete (void* memory, size_t size) {
    s_AllocationMetrics.TotalFreed += size;
    free(memory);
}

static void PrintMemoryUsage() {
    cout << "Memory Usage: " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}

int main()
{
    String str1;                         // Default constructor
    String str2 = "Custom string class"; // Parameterized constructor
    String str3 = str2;                  // Copy constructor
    String str4 = std::move(str2);       // Move constructor
    PrintMemoryUsage();
    assert(str4.size() == str3.size());
    assert(str2.size() == 0);
    PrintMemoryUsage();

    str2 = str3; // Copy assignment
    assert(str2.size() == str3.size());
    PrintMemoryUsage();

    assert(str2[0] == 'C');
    assert(str2[1] == 'u');
    assert(str2[2] == 's');
    assert(str2[3] == 't');
    assert(str2[4] == 'o');
    assert(str2[5] == 'm');

    display(str2);
    PrintMemoryUsage();

    return 0;
}
