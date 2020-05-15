#include "Bitmap.h"

#include "Assertions.h"
#include <stdlib.h>
#include <string.h>

namespace ASL {

Bitmap::Bitmap(u32 width, u32 height)
    : m_width(width)
    , m_height(height)
{
    m_data = (u8*)malloc(width * height);
}

Bitmap::~Bitmap()
{
    ASSERT(m_data);
    free(m_data);
}

void Bitmap::set_data(u8* data)
{
    memcpy(m_data, data, m_width * m_height);
}

} // namespace ASL