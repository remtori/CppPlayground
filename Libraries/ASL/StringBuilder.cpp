#include "StringBuilder.h"

#include "String.h"
#include "StringView.h"

namespace ASL {

StringBuilder::StringBuilder(size_t initial_capacity)
{
    m_buffer.grow(initial_capacity);
}

String StringBuilder::build() const
{
    return to_string();
}

String StringBuilder::to_string() const
{
    return { (const char*)m_buffer.data(), m_length };
}

StringView StringBuilder::string_view() const
{
    return { (const char*)m_buffer.data(), m_length };
}

void StringBuilder::clear()
{
    m_buffer.clear();
    m_length = 0;
}

inline void StringBuilder::will_append(size_t size)
{
    if ((m_length + size) > m_buffer.size())
        m_buffer.grow(max(static_cast<size_t>(16), m_buffer.size() + size));
}

void StringBuilder::append(const StringView& view)
{
    will_append(view.length());
    m_buffer.overwrite(view.characters_wont(), view.length(), m_length);
    m_length += view.length();
}

void StringBuilder::append(char c)
{
    will_append(1);
    m_buffer.data()[m_length] = c;
    m_length += 1;
}

} // namespace ASL