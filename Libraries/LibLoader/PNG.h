#pragma once

#include <ASL/Bitmap.h>
#include <ASL/ByteBuffer.h>
#include <ASL/RefPtr.h>

namespace Loader {

class PNGLoader {
public:
    static bool is_png_image(const ByteBuffer&);
    static RefPtr<Bitmap> load_from_data(const ByteBuffer&);
    static RefPtr<Bitmap> load_from_file(const char* file_path);
};

} // namespace Loader
