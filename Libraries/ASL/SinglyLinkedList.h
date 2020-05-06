#pragma once

#include "Assertions.h"

namespace ASL {

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
        for (Node* p = other.m_head; p != nullptr; p = p->next)
        {
            append(p->value);
        }
    }

    SinglyLinkedList& operator=(const SinglyLinkedList& other)
    {
        clear();
        for (Node* p = other.m_head; p != nullptr; p = p->next)
        {
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
        for (Node* p = m_head; p != nullptr;)
        {
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

    const T& back()
    {
        ASSERT(m_head);
        return m_tail->value;
    }

     void append(const T& value)
    {
        append(T(value));
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

}
