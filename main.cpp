#include <array>
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <map>
#include <unordered_map>
#include <set>

using std::pair;
using std::list;
using std::vector;
using std::hash;
using std::initializer_list;

size_t default_allocate = 5000;

template<class KeyType, class ValueType, class Hash = hash<KeyType>> class HashMap
{
private:
    using pair_type = pair<const KeyType, ValueType>;
    list<pair_type> memory;
    size_t map_size = 0;
    size_t allocated;
    vector<decltype(memory.begin())> pointers;
    Hash hasher;

public:
    explicit HashMap(const Hash & oth, size_t sz = default_allocate) : map_size(0), allocated(sz), hasher(oth){
        pointers.resize(sz);
    }

    explicit HashMap(size_t sz = default_allocate) {
        pointers.resize(sz);
        allocated = sz;
        map_size = 0;
    }

    void clear(size_t sz = default_allocate) {
        memory.clear();
        pointers.clear();
        pointers.resize(sz);
        map_size = 0;
        allocated = sz;
    }

    ValueType& operator [] (const KeyType& key) {
        size_t pos = hasher(key) % allocated;
        if (pointers[pos] == decltype(memory.begin())()) {
            pointers[pos] = memory.insert(memory.end(), {key, ValueType()});
            map_size++;
            return (pointers[pos]->second);
        }
        else {
            auto t = pointers[pos];
            while (t != memory.end() && !(t->first == key) && hasher(t->first) % allocated == pos)
                t++;
            if (t != memory.end() && t->first == key)
                return t->second;
            else {
                map_size++;
                return memory.insert(t, {key, ValueType()})->second;
            }
        }
    }

    const ValueType& at(const KeyType& key) const {
        size_t pos = hasher(key) % allocated;
        if (pointers[pos] == decltype(memory.begin())()) {
            throw std::out_of_range("");
        }
        else {
            auto t = pointers[pos];
            while (t != memory.end() && t->first != key && hasher(t->first) % allocated == pos)
                t++;
            if (t != memory.end() && t->first == key)
                return t->second;
            else {
                throw std::out_of_range("");
            }
        }
    }

    using iterator = typename list<pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename list<pair<const KeyType, ValueType>>::const_iterator;


    iterator begin() {
        return memory.begin();
    }

    iterator end() {
        return memory.end();
    }

    const_iterator begin() const {
        return memory.begin();
    }

    const_iterator end() const {
        return memory.end();
    }

    const_iterator find(const KeyType& key) const {
        size_t pos = hasher(key) % allocated;
        if (pointers[pos] == decltype(memory.begin())()) {
            return end();
        }
        else {
            auto t = pointers[pos];
            while (t != memory.end() && t->first != key && hasher(t->first) % allocated == pos)
                t++;
            if (t != memory.end() && t->first == key)
                return t;
            else {
                return end();
            }
        }
    }

    iterator find(const KeyType& key) {
        size_t pos = hasher(key) % allocated;
        if (pointers[pos] == decltype(memory.begin())()) {
            return end();
        }
        else {
            auto t = pointers[pos];
            while (t != memory.end() && !(t->first == key) && hasher(t->first) % allocated == pos)
                t++;
            if (t != memory.end() && t->first == key)
                return t;
            else {
                return end();
            }
        }
    }

    Hash hash_function() const {
        return hasher;
    }

    void erase(const KeyType& key) {
        auto t = find(key);
        if (t != end()) {
            auto prev = t;
            if (t != begin())
                prev--;
            if (t == begin() || hasher(prev->first) % allocated != hasher(key) % allocated) {
                auto next = t;
                next++;
                if (next == end() || hasher(next->first) % allocated != hasher(key) % allocated)
                    pointers[hasher(key) % allocated] = iterator();
                else
                    pointers[hasher(key) % allocated] = next;
                --map_size;
                memory.erase(t);
                return ;
            }
            memory.erase(t);
            map_size--;
        }
    }

    size_t size() const {
        return map_size;
    }

    bool empty() const {
        return map_size == 0;
    }

    void insert(const pair<KeyType, ValueType> & a) {
        if (find(a.first) == end())
            (*this)[a.first] = a.second;
    }

    template<typename T>
    HashMap(T first, T last, const Hash & oth = Hash(), size_t sz = default_allocate) {
        pointers.resize(sz);
        allocated = sz;
        map_size = 0;
        hasher = oth;
        while (first != last) {
            insert(*first);
            first++;
        }
    }

    HashMap(initializer_list<pair<const KeyType, ValueType>> && a, const Hash & oth = Hash(), size_t sz = default_allocate) {
        auto first = a.begin(), last = a.end();
        pointers.resize(sz);
        allocated = sz;
        map_size = 0;
        hasher = oth;
        while (first != last) {
            insert(*first);
            first++;
        }
    }

    HashMap(const HashMap && a) : memory(a.memory), map_size(a.map_size), allocated(a.allocated), pointers(a.pointers), hasher(a.hasher){}

    HashMap& operator = (const HashMap & a) {
        if (this == &a)
            return *this;
        clear();
        for (auto e : a)
            (*this)[e.first] = e.second;
        return *this;
    }

    HashMap(const HashMap & a) {
        pointers.resize(default_allocate);
        allocated = default_allocate;
        map_size = 0;
        *this = a;
    }
};
