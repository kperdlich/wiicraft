#pragma once

#include <zlib.h>
#include <cstdlib>
#include <cstring>
#include "core.h"

namespace core {
    namespace Zip {
        size_t Decompress(unsigned char* inputData, size_t compressedSize, unsigned char* outputData, size_t decompressedSize)
        {
            z_stream infstream;
            infstream.zalloc = Z_NULL;
            infstream.zfree = Z_NULL;
            infstream.opaque = Z_NULL;
            infstream.total_out = 0;
            infstream.next_in = (Bytef*)inputData;
            infstream.avail_in = (uInt)compressedSize;
            infstream.avail_out = (uInt)decompressedSize - infstream.total_out;
            infstream.next_out = (Bytef*)outputData;

            int initStatus = inflateInit(&infstream);
            ASSERT(initStatus == Z_OK);

            int status = inflate(&infstream, Z_FINISH);
            if (status != Z_STREAM_END)
            {
                if (status == Z_MEM_ERROR)
                    ASSERT(false);

                if (status == Z_OK)
                    ASSERT(false);

                if (status == Z_STREAM_ERROR)
                    ASSERT(false);

                if (status == Z_NEED_DICT)
                    ASSERT(false);

                if (status == Z_ERRNO)
                    ASSERT(false);

                if (status == Z_DATA_ERROR)
                    ASSERT(false);

                if (status == Z_BUF_ERROR)
                    ASSERT(false);

                ASSERT(false);
            }
            //ASSERT(status == Z_STREAM_END);

            inflateEnd(&infstream);
            return infstream.total_out;
        }
    }
}
