#pragma once

#include <ASL/Bitmap.h>
#include <ASL/ByteBuffer.h>
#include <ASL/RefPtr.h>

namespace Codec {

static LIB_API bool is_png_image(const ByteBuffer&);
static LIB_API RefPtr<Bitmap> load_png_from_data(ByteBuffer&);
static LIB_API RefPtr<Bitmap> load_png_from_file(const char* file_path);

} // namespace Codec
