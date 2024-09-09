#include "ring-buffer.h"

#include <cassert>
#include <iostream>

void run_test_cases()
{
    RingBuffer<int, 10> buffer;

    // Test capacity, isEmpty, and isFull behavior
    assert(buffer.capacity() == 10);
    assert(buffer.isEmpty());
    assert(!buffer.isFull());

    // Test exception when retrieving from an empty buffer
    try {
        buffer.get();
        assert(false); // Should not reach this point
    } catch (const std::out_of_range &e) {
        std::cout << "Caught expected exception for empty buffer: " << e.what()
                  << '\n';
    }

    // Fill the buffer and verify it's full
    for (int i = 0; i < 10; ++i) {
        buffer.put(i);
    }

    assert(buffer.isFull());
    assert(buffer.size() == 10);

    // Retrieve the first element and check size decrement
    assert(buffer.get() == 0);
    assert(buffer.size() == 9);
    assert(!buffer.isFull());

    // Insert more elements and test wraparound behavior
    buffer.put(10);
    assert(buffer.get() == 1); // Second item inserted originally
    assert(buffer.size() == 9);

    // Verify continued retrievals
    assert(buffer.get() == 2);
    assert(buffer.size() == 8);

    std::cout << "All RingBuffer tests passed successfully.\n";
}

int main()
{
    run_test_cases();
    return 0;
}
