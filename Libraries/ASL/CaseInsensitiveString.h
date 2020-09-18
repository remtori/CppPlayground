#pragma once

#include <string>

// TODO: Custom implementation to switch the internal data storage as lower/upper case
namespace ASL {

struct case_insensitive_char_traits : public ::std::char_traits<char> {
    static bool eq(char c1, char c2) { return toupper(c1) == toupper(c2); }
    static bool ne(char c1, char c2) { return toupper(c1) != toupper(c2); }
    static bool lt(char c1, char c2) { return toupper(c1) < toupper(c2); }
    static int compare(const char* s1, const char* s2, size_t n)
    {
        while (n-- != 0) {
            if (toupper(*s1) < toupper(*s2))
                return -1;
            if (toupper(*s1) > toupper(*s2))
                return 1;
            ++s1;
            ++s2;
        }
        return 0;
    }

    static const char* find(const char* s, int n, char a)
    {
        while (n-- > 0 && toupper(*s) != toupper(a)) {
            ++s;
        }
        return s;
    }
};

using CaseInsenitiveString = ::std::basic_string<char, case_insensitive_char_traits>;

} // namespace ASL

using ASL::CaseInsenitiveString;

template<>
struct std::hash<::ASL::CaseInsenitiveString> {
    size_t operator()(const CaseInsenitiveString& s) const noexcept
    {
        size_t hash = 0;

        for (size_t i = 0; i < s.size(); i++) {
            hash += ::toupper(s[i]);
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }

        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);

        return hash;
    }
};