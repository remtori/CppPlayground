#pragma once

#include "Assertions.h"
#include "SinglyLinkedList.h"
#include "Traits.h"

namespace ASL {

template<typename HashTableType, typename ElementType, typename BucketIteratorType>
class HashTableIterator {
public:
    bool operator!=(const HashTableIterator& other) const
    {
        if (m_is_end && other.m_is_end)
            return false;

        return &m_table != &other.m_table
            || m_is_end != other.m_is_end
            || m_bucket_index != other.m_bucket_index
            || m_bucket_iterator != other.m_bucket_iterator;
    }

    bool operator==(const HashTableIterator& other) const { return !(*this != other); }

    ElementType& operator*() { return *m_bucket_iterator; }
    ElementType* operator->() { return m_bucket_iterator.operator->(); }

    HashTableIterator& operator++()
    {
        skip_to_next();
        return *this;
    }

    void skip_to_next()
    {
        while (!m_is_end) {
            if (m_bucket_iterator.is_end()) {
                ++m_bucket_index;
                if (m_bucket_index >= m_table.capacity()) {
                    m_is_end = true;
                    return;
                }

                m_bucket_iterator = m_table.bucket(m_bucket_index).begin();
            } else {
                ++m_bucket_iterator;
            }

            if (!m_bucket_iterator.is_end())
                return;
        }
    }

private:
    friend HashTableType;

    explicit HashTableIterator(HashTableType& table, bool is_end, BucketIteratorType bucket_iterator = {}, size_t bucket_index = 0)
        : m_table(table)
        , m_bucket_iterator(bucket_iterator)
        , m_bucket_index(bucket_index)
        , m_is_end(is_end)
    {
        if (!is_end && !m_table.is_empty() && m_bucket_iterator.is_end()) {
            m_bucket_iterator = m_table.bucket(0).begin();
            if (m_bucket_iterator.is_end())
                skip_to_next();
        }
    }

    HashTableType& m_table;
    BucketIteratorType m_bucket_iterator;
    size_t m_bucket_index = 0;
    bool m_is_end = false;
};

template<typename T, typename TraitsForT = Traits<T>>
class HashTable {
    using Bucket = SinglyLinkedList<T>;

public:
    HashTable() {}

    HashTable(const HashTable& other)
    {
        ensure_capacity(other.size());
        for (auto& it : other)
            set(it);
    }

    HashTable& operator=(const HashTable& other)
    {
        if (this != &other) {
            clear();
            ensure_capacity(other.size());
            for (auto& it : other)
                set(it);
        }
        return *this;
    }

    HashTable(HashTable&& other)
        : m_buckets(other.m_buckets)
        , m_capacity(other.m_capacity)
        , m_size(other.m_size)
    {
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_buckets = nullptr;
    }

    HashTable& operator=(HashTable&& other)
    {
        if (this != &other) {
            clear();
            m_buckets = other.m_buckets;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_size = 0;
            other.m_capacity = 0;
            other.m_buckets = nullptr;
        }
        return *this;
    }

    ~HashTable() { clear(); }

    bool is_empty() const { return m_size == 0; }
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }

    void set(const T& value)
    {
        if (!m_capacity)
            ensure_capacity(1);

        auto& bucket = lookup(value);
        for (auto& e : bucket) {
            if (TraitsForT::equals(e, value)) {
                e = value;
                return;
            }
        }

        if (m_size > m_capacity) {
            ensure_capacity(m_size + 1);
            insert(value);
        } else {
            bucket.append(value);
        }

        ++m_size;
    }

    void set(T&& value)
    {
        if (!m_capacity)
            ensure_capacity(1);

        auto& bucket = lookup(value);
        for (auto& e : bucket) {
            if (TraitsForT::equals(e, value)) {
                e = move(value);
                return;
            }
        }

        if (m_size > m_capacity) {
            ensure_capacity(m_size + 1);
            insert(move(value));
        } else {
            bucket.append(move(value));
        }

        ++m_size;
    }

    bool contains(const T& value) const
    {
        if (is_empty())
            return false;

        auto& bucket = lookup(value);
        for (auto& e : bucket) {
            if (TraitsForT::equals(e, value))
                return true;
        }

        return false;
    }

    void clear()
    {
        if (m_buckets) {
            delete[] m_buckets;
            m_buckets = nullptr;
        }

        m_capacity = 0;
        m_size = 0;
    }

    using Iterator = HashTableIterator<HashTable, T, typename Bucket::Iterator>;
    friend Iterator;
    Iterator begin() { return Iterator(*this, is_empty()); }
    Iterator end() { return Iterator(*this, true); }

    using ConstIterator = HashTableIterator<const HashTable, const T, typename Bucket::ConstIterator>;
    friend ConstIterator;
    ConstIterator begin() const { return ConstIterator(*this, is_empty()); }
    ConstIterator end() const { return ConstIterator(*this, true); }

    template<typename Finder>
    Iterator find(unsigned hash, Finder finder)
    {
        if (is_empty())
            return end();

        size_t bucket_index;
        auto& bucket = lookup_with_hash(hash, &bucket_index);

        auto bucket_iterator = bucket.find(finder);
        if (bucket_iterator != bucket.end())
            return Iterator(*this, false, bucket_iterator, bucket_index);

        return end();
    }

    template<typename Finder>
    ConstIterator find(unsigned hash, Finder finder) const
    {
        if (is_empty())
            return end();

        size_t bucket_index;
        auto& bucket = lookup_with_hash(hash, &bucket_index);

        auto bucket_iterator = bucket.find(finder);
        if (bucket_iterator != bucket.end())
            return ConstIterator(*this, false, bucket_iterator, bucket_index);

        return end();
    }

    Iterator find(const T& value)
    {
        return find(TraitsForT::hash(value), [&](auto& other) { return TraitsForT::equals(value, other); });
    }

    ConstIterator find(const T& value) const
    {
        return find(TraitsForT::hash(value), [&](auto& other) { return TraitsForT::equals(value, other); });
    }

    void remove(const T& value)
    {
        auto it = find(value);
        if (it != end())
            remove(it);
    }

    void remove(Iterator it)
    {
        ASSERT(!is_empty());
        m_buckets[it.m_bucket_index].remove(it.m_bucket_iterator);
        --m_size;
    }

    void ensure_capacity(size_t new_capacity)
    {
        if (new_capacity < m_size)
            return;

        new_capacity *= 2;
        auto* new_buckets = new Bucket[new_capacity];
        auto* old_buckets = m_buckets;

        auto old_capacity = m_capacity;
        m_buckets = new_buckets;
        m_capacity = new_capacity;

        for (size_t i = 0; i < old_capacity; ++i) {
            for (auto& v : old_buckets[i])
                insert(move(v));
        }

        delete[] old_buckets;
    }

private:
    Bucket& bucket(size_t index)
    {
        return m_buckets[index];
    }

    const Bucket& bucket(size_t index) const
    {
        return m_buckets[index];
    }

    Bucket& lookup(const T& value, size_t* bucket_index = nullptr)
    {
        unsigned hash = TraitsForT::hash(value);
        if (bucket_index)
            *bucket_index = hash % m_capacity;
        return m_buckets[hash % m_capacity];
    }

    const Bucket& lookup(const T& value, size_t* bucket_index = nullptr) const
    {
        unsigned hash = TraitsForT::hash(value);
        if (bucket_index)
            *bucket_index = hash % m_capacity;
        return m_buckets[hash % m_capacity];
    }

    Bucket& lookup_with_hash(unsigned hash, size_t* bucket_index = nullptr)
    {
        if (bucket_index)
            *bucket_index = hash % m_capacity;
        return m_buckets[hash % m_capacity];
    }

    const Bucket& lookup_with_hash(unsigned hash, size_t* bucket_index = nullptr) const
    {
        if (bucket_index)
            *bucket_index = hash % m_capacity;
        return m_buckets[hash % m_capacity];
    }

    void insert(const T& value)
    {
        auto& bucket = lookup(value);
        bucket.append(value);
    }

    void insert(T&& value)
    {
        auto& bucket = lookup(value);
        bucket.append(move(value));
    }

    Bucket* m_buckets = nullptr;
    size_t m_capacity = 0;
    size_t m_size = 0;
};

} // namespace ASL

using ASL::HashTable;