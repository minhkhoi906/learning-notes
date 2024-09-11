#include "queue.h"

#include <cassert>
#include <utility>

using namespace std;

void RunTestCases()
{
    Queue<int> queue_1;
    Queue<float> queue_2;

    assert(queue_1.size() == 0);
    assert(queue_2.size() == 0);

    // Parameterized ctor
    Queue<int> queue_3(4, 1);
    assert(queue_3.size() == 4);
    assert(queue_3.front() == 1);
    // Copy ctor
    Queue<int> queue_4 = queue_3;
    assert(queue_4.size() == 4);
    assert(queue_4.front() == 1);
    // Move ctor
    Queue<int> queue_5 = std::move(queue_3);
    assert(queue_5.size() == 4);
    assert(queue_3.size() == 0);

    // Test case for Methods
    queue_1.enqueue(1);
    queue_1.enqueue(2);
    queue_1.enqueue(3);
    assert(queue_1.size() == 3);

    assert(queue_1.front() == 1);
    queue_1.dequeue();
    assert(queue_1.front() == 2);
    assert(queue_1.size() == 2);

    cout << "Passed All Test Cases!\n";
}