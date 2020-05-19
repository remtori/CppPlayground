#pragma once

namespace ASL {

/**
 * Badge is a wrapper around T that can only be constructed by T
 *
 * Which mean we can utilize this as an guard
 * to make an public method can only be call
 * inside another specific class
 */
template<typename T>
class Badge {
    friend class T;

private:
    Badge() {}

    Badge(const Badge&) = delete;
    Badge& operator=(const Badge&) = delete;

    Badge(Badge&&) = delete;
    Badge& operator=(Badge&&) = delete;
};

} // namespace ASL

using ASL::Badge;
