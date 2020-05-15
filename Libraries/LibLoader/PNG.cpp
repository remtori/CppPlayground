#include "PNG.h"

#include <stdio.h>
#include <stdlib.h>

namespace Loader {

static const char png_header[] = { (char)0x89, 'P', 'N', 'G', '\r', '\n', ' ', '\n' };

bool PNGLoader::is_png_image(const ByteBuffer& buffer)
{
    if (buffer.size() < 8)
        return false;

    return buffer.sub_equals((const char*)png_header, sizeof(png_header));
}

RefPtr<Bitmap> PNGLoader::load_from_data(const ByteBuffer& buffer)
{
    if (!is_png_image(buffer))
        return nullptr;
}

RefPtr<Bitmap> PNGLoader::load_from_file(const char* file_path)
{
    // TODO: Use Platform dependent ways to get the file size
    // And then maybe make a general FileReader/FileWriter to abstract thing away

    FILE* f = fopen(file_path, "r");
    if (f == nullptr)
        return nullptr;

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    ByteBuffer buffer(size);
    fread(buffer.data(), 1, size, f);
    fclose(f);

    return load_from_data(buffer);
}

} // namespace Loader
