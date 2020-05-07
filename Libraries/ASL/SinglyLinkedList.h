#pragma once

#include "Assertions.h"
#include "StdLibExtras.h"

namespace ASL {

template<typename ListType, typename ElementType>
class SinglyLinkedListIterator {
public:
    SinglyLinkedListIterator() {}

    bool operator==(const SinglyLinkedListIterator& other) const { return m_node == other.m_node; }
    bool operator!=(const SinglyLinkedListIterator& other) const { return m_node != other.m_node; }

    SinglyLinkedListIterator& operator++()
    {
        m_prev = m_node;
        m_node = m_node->next;
        return *this;
    }

    ElementType& operator*() { return m_node->value; }
    ElementType* operator->() { return &m_node->value; }

    bool is_end() const { return !m_node; }
    bool is_begin() const { return !m_prev; }

private:
    friend ListType;
    explicit SinglyLinkedListIterator(typename ListType::Node* node, typename ListType::Node* prev = nullptr)
        : m_node(node)
        , m_prev(prev)
    {
    }

    typename ListType::Node* m_node = nullptr;
    typename ListType::Node* m_prev = nullptr;
};

template<typename T>
class SinglyLinkedList {
private:
    struct Node {
        explicit Node(T&& v)
            : value(move(v))
        {
        }
        explicit Node(const T& v)
            : value(v)
        {
        }
        T value;
        Node* next = nullptr;
    };

public:
    SinglyLinkedList() {}

    SinglyLinkedList(const SinglyLinkedList& other)
    {
        for (Node* p = other.m_head; p != nullptr; p = p->next) {
            append(p->value);
        }
    }

    SinglyLinkedList& operator=(const SinglyLinkedList& other)
    {
        clear();
        for (Node* p = other.m_head; p != nullptr; p = p->next) {
            append(p->value);
        }
    }

    SinglyLinkedList(SinglyLinkedList&& other)
        : m_head(other.m_head)
        , m_tail(other.m_tail)
    {
    }

    SinglyLinkedList& operator=(SinglyLinkedList&& other)
    {
        m_head = other.m_head;
        m_tail = other.m_tail;
        other.m_head = nullptr;
        other.m_tail = nullptr;
    }

    ~SinglyLinkedList()
    {
        clear();
    }

    void clear()
    {
        for (Node* p = m_head; p != nullptr;) {
            Node* next = p->next;
            delete p;
            p = next;
        }

        m_head = nullptr;
        m_tail = nullptr;
    }

    bool is_empty() const { return m_head == nullptr; }

    inline size_t size_slow() const
    {
        size_t size = 0;
        for (Node* p = m_head; p != nullptr; p = p->next)
            ++size;
        return size;
    }

    T& front()
    {
        ASSERT(m_head);
        return m_head->value;
    }

    const T& front() const
    {
        ASSERT(m_head);
        return m_head->value;
    }

    T& back()
    {
        ASSERT(m_head);
        return m_tail->value;
    }

    const T& back() const
    {
        ASSERT(m_head);
        return m_tail->value;
    }

    void append(const T& value)
    {
        append(move(T(value)));
    }

    void append(T&& value)
    {
        auto* node = new Node(move(value));
        if (!m_head) {
            m_head = node;
            m_tail = node;
            return;
        }
        m_tail->next = node;
        m_tail = node;
    }

    bool contains_slow(const T& value) const
    {
        for (auto* node = m_head; node; node = node->next) {
            if (node->value == value)
                return true;
        }
        return false;
    }

    using Iterator = SinglyLinkedListIterator<SinglyLinkedList, T>;
    friend Iterator;
    Iterator begin() { return Iterator(m_head); }
    Iterator end() { return {}; }

    using ConstIterator = SinglyLinkedListIterator<const SinglyLinkedList, const T>;
    friend ConstIterator;
    ConstIterator begin() const { return ConstIterator(m_head); }
    ConstIterator end() const { return {}; }

    template<typename Finder>
    ConstIterator find(Finder finder) const
    {
        Node* prev = nullptr;
        for (auto* node = m_head; node; node = node->next) {
            if (finder(node->value))
                return ConstIterator(node, prev);
            prev = node;
        }
        return end();
    }

    template<typename Finder>
    Iterator find(Finder finder)
    {
        Node* prev = nullptr;
        for (auto* node = m_head; node; node = node->next) {
            if (finder(node->value))
                return Iterator(node, prev);
            prev = node;
        }
        return end();
    }

    ConstIterator find(const T& value) const
    {
        return find([&](auto& other) { return value == other; });
    }

    Iterator find(const T& value)
    {
        return find([&](auto& other) { return value == other; });
    }

    void remove(Iterator iterator)
    {
        ASSERT(!iterator.is_end());
        if (m_head == iterator.m_node)
            m_head = iterator.m_node->next;
        if (m_tail == iterator.m_node)
            m_tail = iterator.m_prev;
        if (iterator.m_prev)
            iterator.m_prev->next = iterator.m_node->next;
        delete iterator.m_node;
    }

    void insert_before(Iterator iterator, const T& value)
    {
        insert_before(iterator, T(value));
    }

    void insert_before(Iterator iterator, T&& value)
    {
        auto* node = new Node(move(value));
        node->next = iterator.m_node;
        if (m_head == iterator.m_node)
            m_head = node;
        if (iterator.m_prev)
            iterator.m_prev->next = node;
    }

    void insert_after(Iterator iterator, const T& value)
    {
        insert_after(iterator, T(value));
    }

    void insert_after(Iterator iterator, T&& value)
    {
        if (iterator.is_end()) {
            append(value);
            return;
        }

        auto* node = new Node(move(value));
        node->next = iterator.m_node->next;

        iterator.m_node->next = node;

        if (m_tail == iterator.m_node)
            m_tail = node;
    }

    bool operator==(const SinglyLinkedList& other)
    {
        return m_head == other.m_head;
    }

    bool operator!=(const SinglyLinkedList& other)
    {
        return m_head != other.m_head;
    }

private:
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
};

} // namespace ASL

using ASL::SinglyLinkedList;
