#pragma once

#include "HashTable.h"
#include "Optional.h"
#include "Traits.h"
#include "Vector.h"

namespace ASL {

/**
 * This is just a wrapper around HashTable so its pretty straight forward to implement
 * And for once we dont need to override the default move/copy constructor/operator
 */
template<typename K, typename V, typename KeyTraits = Traits<K>>
class HashMap {
private:
    struct Entry {
        K key;
        V value;
    };

    struct EntryTraits {
        static unsigned hash(const Entry& entry) { return KeyTraits::hash(entry.key); }
        static bool equals(const Entry& a, const Entry& b) { return KeyTraits::equals(a.key, b.key); }
    };

public:
    HashMap() {}

    bool is_empty() const { return m_table.is_empty(); }
    size_t size() const { return m_table.size(); }
    size_t capacity() const { return m_table.capacity(); }
    void clear() { m_table.clear(); }

    void set(const K& key, const V& value) { m_table.set({ key, value }); }
    void set(const K& key, V&& value) { m_table.set({ key, move(value) }); }

    void remove(const K& key)
    {
        auto it = find(key);
        if (it != end())
            m_table.remove(it);
    }

    using HashTableType = HashTable<Entry, EntryTraits>;
    using IteratorType = typename HashTableType::Iterator;
    using ConstIteratorType = typename HashTableType::ConstIterator;

    IteratorType begin() { return m_table.begin(); }
    IteratorType end() { return m_table.end(); }

    IteratorType find(const K& key)
    {
        return m_table.find(KeyTraits::hash(key), [&](auto& entry) { return KeyTraits::equals(key, entry.key); });
    }

    template<typename Finder>
    IteratorType find(unsigned hash, Finder finder)
    {
        return m_table.find(hash, finder);
    }

    ConstIteratorType begin() const { return m_table.begin(); }
    ConstIteratorType end() const { return m_table.end(); }

    ConstIteratorType find(const K& key) const
    {
        return m_table.find(KeyTraits::hash(key), [&](auto& entry) { return KeyTraits::equals(key, entry.key); });
    }

    template<typename Finder>
    ConstIteratorType find(unsigned hash, Finder finder) const
    {
        return m_table.find(hash, finder);
    }

    void ensure_capacity(size_t capacity) { m_table.ensure_capacity(capacity); }

    Optional<typename Traits<V>::PeekType> get(const K& key) const
    {
        auto it = find(key);
        if (it == end())
            return {};
        return (*it).value;
    }

    V get_or(const K& key, const V& alternative)
    {
        auto it = find(key);
        if (it == end())
            return alternative;
        return (*it).value;
    }

    bool contains(const K& key) const
    {
        return find(key) != end();
    }

    void remove(IteratorType it)
    {
        m_table.remove(it);
    }

    V& ensure(const K& key)
    {
        auto it = find(key);
        if (it == end())
            set(key, V());
        return find(key)->value;
    }

    Vector<K> keys() const
    {
        Vector<K> list;
        list.ensure_capacity(size());
        for (auto& it : *this)
            list.unchecked_append(it.key);
        return list;
    }

private:
    HashTable<Entry, EntryTraits> m_table;
};

} // namespace ASL

using ASL::HashMap;
