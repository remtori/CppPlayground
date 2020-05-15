#pragma once

#include "NonnullRefPtr.h"
#include "RefCounted.h"
#include "Types.h"

namespace ASL {

class Bitmap : public RefCounted<Bitmap> {
public:
    static NonnullRefPtr<Bitmap> create(u32 width, u32 height)
    {
        return adopt(*new Bitmap(width, height));
    }

    Bitmap(const Bitmap& bitmap)
        : m_width(bitmap.m_width)
        , m_height(bitmap.m_height)
    {
        set_data(bitmap.m_data);
    }

    Bitmap(Bitmap&& bitmap)
        : m_width(bitmap.m_width)
        , m_height(bitmap.m_height)
    {
        m_data = bitmap.m_data;
        bitmap.m_width = 0;
        bitmap.m_height = 0;
        bitmap.m_data = nullptr;
    }

    ~Bitmap();

    void set_data(u8*);

    inline const u8* data() const { return m_data; }
    inline u8* data() { return m_data; }

    inline u32 width() const { return m_width; }
    inline u32 height() const { return m_height; }

private:
    Bitmap(u32 width, u32 height);

    u8* m_data = nullptr;
    u32 m_width = 0;
    u32 m_height = 0;
};

} // namespace ASL

using ASL::Bitmap;
