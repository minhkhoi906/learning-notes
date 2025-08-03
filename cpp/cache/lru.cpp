#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

struct LRU {
    LRU(size_t capacity) : _capacity(capacity) {}
    int get(int key)
    {
        auto it = _lookup.find(key);
        if (it != _lookup.end()) {
            _store.splice(_store.begin(), _store , it->second);
            return it->second->second;
        }

        return -1;
    }

    void put(int key, int value)
    {
        auto it = _lookup.find(key);
        if (it != _lookup.end()) {
            _store.splice(_store.begin(), _store , it->second);
            it->second->second = value;
            return;
        }
        if (_store.size() == _capacity) {
            _lookup.erase(_store.back().first);
            _store.pop_back();
        }

        _store.push_front(std::pair{key, value});
        _lookup[key] = _store.begin();

    }

  private:
    using ItemList = list<pair<int, int>>;
    size_t _capacity;
    ItemList _store;
    unordered_map<int, ItemList::iterator> _lookup;
};

int main()
{
    {
        LRU cache(2);

        cache.put(1, 1);
        assert(cache.get(1) == 1);

        cache.put(2, 2);
        assert(cache.get(2) == 2);

        cache.put(3, 3); // evicts key 1
        assert(cache.get(1) == -1);

        cache.put(2, 4);
        assert(cache.get(2) == 4);

        cache.put(4, 4); // evicts key 3
        assert(cache.get(3) == -1);

        assert(cache.get(5) == -1);
    }
    {
        LRU cache(1);

        cache.put(1, 1);
        assert(cache.get(1) == 1); // returns 1

        cache.put(2, 2); // evicts key 1
        assert(cache.get(1) == -1);
        assert(cache.get(2) == 2);

        cache.put(2, 3);
        assert(cache.get(2) == 3);
    }
    {
        LRU cache(3);

        cache.put(1, 1);
        cache.put(2, 2);
        cache.put(3, 3);
        assert(cache.get(1) == 1);
        assert(cache.get(2) == 2);
        assert(cache.get(3) == 3);

        cache.put(4, 4); // evicts key 1
        assert(cache.get(1) == -1);
        assert(cache.get(4) == 4);

        assert(cache.get(2) == 2);
        cache.put(5, 5); // evicts key 3 as key 2 was accessed recently
        assert(cache.get(3) == -1);
    }
    return 0;
}