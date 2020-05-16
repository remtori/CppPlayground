#include "PNG.h"

#include "CRC.h"
#include <ASL/LogStream.h>
#include <ASL/NumericLimits.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Codec {

static const size_t npos = NumericLimits<size_t>::max();
static const char png_header[] = { (char)0x89, 'P', 'N', 'G', '\r', '\n', ' ', '\n' };

struct ImageMetadata {
    u32 width;
    u32 height;
    u8 bit_depth;
    u8 color_type;
    u8 compression_method;
    u8 filter_method;
    u8 interlace_method;
};

bool PNGLoader::is_png_image(const ByteBuffer& buffer)
{
    if (buffer.size() <= 8)
        return false;

    return buffer.sub_equals((const char*)png_header, sizeof(png_header));
}

size_t decode_PLTE_chunk(const ByteBuffer& buffer, size_t index, size_t chunk_length, RefPtr<Bitmap> image);
size_t decode_IDAT_chunk(const ByteBuffer& buffer, size_t index, size_t chunk_length, RefPtr<Bitmap> image);

RefPtr<Bitmap> PNGLoader::load_from_data(const ByteBuffer& buffer)
{
    if (!is_png_image(buffer))
        return nullptr;

    auto is_chunk_ok = [&](size_t chunk_type_index, size_t chunk_length) {
        // The CRC is calculated over the chunk type and chunk data, but not the length
        u32 data_crc = compute_crc_32(&buffer.data()[chunk_type_index], chunk_length + 4);
        u32 crc32;
        buffer.read(&crc32, 4, chunk_type_index + chunk_length + 4);
        bool ok = crc32 == data_crc;
        if (!ok)
            dbg("PNG Decoder") << "Check sum failed at offset=" << chunk_type_index - 4;

        return ok;
    };

    size_t index = 8;
    size_t chunk_type_index = 0;
    u32 chunk_length = 0;
    char chunk_type[4];
    ImageMetadata* metadata = nullptr;
    RefPtr<Bitmap> image = nullptr;

    while (index < buffer.size()) {
        buffer.read(&chunk_length, 4, index);
        index += 4;

        chunk_type_index = index;
        buffer.read(chunk_type, 4, index);
        index += 4;

        // For now we only process critical chunk
        // If the first letter of chunk type is lower case its a ancillary chunk
        // we just do the checksum them skip
        if (islower(chunk_type[0])) {
            if (!is_chunk_ok(chunk_type_index, chunk_length))
                return nullptr;
            continue;
        }

        if (!strcmp(chunk_type, "IHDR")) {
            ASSERT(!metadata);
            ASSERT(chunk_length == 13);
            metadata = new ImageMetadata;

            buffer.read(&metadata->width, 4, index);
            index += 4;
            buffer.read(&metadata->height, 4, index);
            index += 4;

            buffer.read(&metadata->bit_depth, 1, index++);
            buffer.read(&metadata->color_type, 1, index++);
            buffer.read(&metadata->compression_method, 1, index++);
            buffer.read(&metadata->filter_method, 1, index++);
            buffer.read(&metadata->interlace_method, 1, index++);

            image = Bitmap::create(metadata->width, metadata->height);

        } else if (metadata == nullptr) {
            dbg("PNG Decoder") << "Decoding failed because the first chunk is not IHDR";
            return nullptr;
        } else if (!strcmp(chunk_type, "PLTE")) {
            index = decode_PLTE_chunk(buffer, index, chunk_length, image);
            if (index == npos)
                return nullptr;
        } else if (!strcmp(chunk_type, "IDAT")) {
            index = decode_IDAT_chunk(buffer, index, chunk_length, image);
            if (index == npos)
                return nullptr;
        }

        if (!is_chunk_ok(chunk_type_index, chunk_length))
            return nullptr;

        if (!strcmp(chunk_type, "IEND")) {
            return image;
        }

        index += 4;
    }

    dbg("PNG Decoder") << "The image does not have a graceful IEND chunk but its ok :)";
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

size_t decode_PLTE_chunk(const ByteBuffer& buffer, size_t index, size_t chunk_length, RefPtr<Bitmap> image)
{
    if (chunk_length % 3) {
        dbg("PNG Decoder") << "Decoding failed because PLTE chunk length=" << chunk_length << " is not divisible by 3";
        return npos;
    }
}

size_t decode_IDAT_chunk(const ByteBuffer& buffer, size_t index, size_t chunk_length, RefPtr<Bitmap> image)
{
}

} // namespace Codec
