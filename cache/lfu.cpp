#include <cstddef>
#include <list>
#include <unordered_map>

/* Problem:
Implement an LFU cache data structure with a limited capacity
that provides two O(1) (average) methods:
- int get(int key)
- void put(int key, int value)

When the cache runs out of capacity, the put method should
drop the least frequently used key (both get and put calls
count as use), if there is a tie, the least recently
used key should be dropped.
*/

/* Solution:

For our LFU cache, we need to be able to drop the least frequently
used key (in O(1)), meaning that we must keep the keys in sorted order.
On top of that, since many keys can have the same frequency, we must
keep the keys with the same frequency in the least recently used order.

When we access a key (either through get or put), we also need to be
able to relocate it within our sorted data structure in O(1).

Potentially this relocation may need to skip over many other keys.

These features heavily limit what possibilities we have. We must use
a std::list for the fast relocation of elements, and we can combine
that with a std::unordered_map to look up information based on a key.

However, we need one more trick. We must bucket the data by frequency
to relocate keys across many other keys. Moving a key to a higher
frequency, we can then relocate it to the bucket with frequency+1,
making sure to keep around only buckets that are not empty (limiting
the number of buckets to the LFU cache capacity).
*/

struct Bucket {
    int freq;
    std::list<int> keys;
};

struct Item {
    int value;
    std::list<Bucket>::iterator bucket;
    std::list<int>::iterator position;
};

struct LFU {
    // Create a LFU cache with the specified capacity
    LFU(int capacity) : capacity_(capacity) {}

    // Get the value for the given key
    // - return -1 if the key is not present in the cache
    // - increase the use count for this key
    int get(int key)
    {
        auto it = lookup_.find(key);
        if (it == lookup_.end())
            return -1;
        // Relocate the key and store the new location
        lookup_[key] = increment(key, it->second.value);
        return it->second.value;
    }

    // Set the value for a key
    // - if the key is new, it starts with use count 1
    //   and if the cache is at capacity, drop the LFU key
    // - if the key already exists, increase the use count
    void put(int key, int value)
    {
        // Relocate the key and store the new location
        lookup_[key] = increment(key, value);
    }

  private:
    Item increment(int key, int value)
    {
        auto it = lookup_.find(key);
        // This a new key
        if (it == lookup_.end()) {
            // If we are at capacity, drop the LFU key
            if (capacity_ == 0) {
                drop_least();
                ++capacity_;
            }
            // Either create or get the bucket for frequency == 1
            auto bucket = get_one_bucket();
            // Keep the keys in least-recently-used order
            bucket->keys.push_back(key);
            --capacity_;
            // Return the position information:
            // - bucket and position within the bucket
            return {value, bucket, std::prev(bucket->keys.end())};
        } else {
            // This key already exists, we are just updating

            // Either create or get the bucket for frequency+1
            auto bucket = get_next_bucket(it->second.bucket);
            // Keep the keys in least-recently-used order
            bucket->keys.push_back(key);
            // Remove the key from the old bucket
            it->second.bucket->keys.erase(it->second.position);
            // If the bucket is now empty, remove it as well
            maybe_drop_bucket(it->second.bucket);
            // Return the position information:
            // - bucket and position within the bucket
            return {value, bucket, std::prev(bucket->keys.end())};
        }
    }

    // Either create or get the bucket for frequency == 1
    std::list<Bucket>::iterator get_one_bucket()
    {
        if (buckets_.empty() || buckets_.front().freq != 1) {
            buckets_.push_front({1, {}});
        }
        return buckets_.begin();
    }

    // Either create or get the bucket for frequency + 1
    std::list<Bucket>::iterator get_next_bucket(std::list<Bucket>::iterator curr)
    {
        auto next = std::next(curr);
        if (next == buckets_.end() || curr->freq + 1 < next->freq) {
            next = buckets_.insert(next, {curr->freq + 1, {}});
        }
        return next;
    }

    // If the bucket is empty, remove it
    void maybe_drop_bucket(std::list<Bucket>::iterator bucket)
    {
        if (bucket->keys.empty())
            buckets_.erase(bucket);
    }

    // Drop the LFU key
    void drop_least()
    {
        // buckets_.front() is the bucket with the least frequency
        // buckets_.front().keys.front() is the least recently
        // used key within this bucket
        int key = buckets_.front().keys.front();
        buckets_.front().keys.pop_front();
        // Also drop from the lookup map
        lookup_.erase(key);
        // If the bucket is now empty, remove it as well
        maybe_drop_bucket(buckets_.begin());
    }

    size_t capacity_;
    // Map: key -> { value, bucket, position within bucket }
    std::unordered_map<int, Item> lookup_;
    // List of buckets
    std::list<Bucket> buckets_;
};

#include <cassert>

int main()
{
    LFU a(3);
    a.put(1, 2);
    a.put(2, 3);
    a.put(3, 4);
    a.put(4, 5);
    assert(a.get(1) == -1);
    assert(a.get(2) == 3);
    assert(a.get(3) == 4);
    assert(a.get(4) == 5);
    a.put(2, 0);
    a.put(2, 0);
    a.put(5, 1);
    assert(a.get(3) == -1);
    a.put(6, 2);
    assert(a.get(5) == -1);
    assert(a.get(2) == 0);
    assert(a.get(6) == 2);
    assert(a.get(4) == 5);

    LFU b(2);
    b.put(1, 1);
    b.put(2, 2);
    assert(b.get(1) == 1);
    b.put(3, 3);
    assert(b.get(2) == -1);
    assert(b.get(3) == 3);
    b.put(4, 4);
    assert(b.get(1) == -1);
    assert(b.get(3) == 3);
    assert(b.get(4) == 4);
}