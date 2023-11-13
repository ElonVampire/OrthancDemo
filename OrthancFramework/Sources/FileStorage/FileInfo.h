
#pragma once

#include "../OrthancFramework.h"
#include "../Enumerations.h"

#include <stdint.h>

namespace Orthanc
{
    struct ORTHANC_PUBLIC FileInfo
    {
    private:
        bool            valid_;
        std::string     uuid_;
        FileContentType contentType_;
        uint64_t        uncompressedSize_;
        std::string     uncompressedMD5_;
        CompressionType compressionType_;
        uint64_t        compressedSize_;
        std::string     compressedMD5_;

    public:
        FileInfo();

        /**
         * Constructor for an uncompressed attachment.
         */
        FileInfo(const std::string& uuid,
                 FileContentType contentType,
                 uint64_t size,
                 const std::string& md5);

        /**
         * Constructor for an compressed attachment.
         */
        FileInfo(const std::string& uuid,
                 FileContentType contentType,
                 uint64_t uncompressedSize,
                 const std::string& uncompressedMd5,
                 CompressionType compressionType,
                 uint64_t compressedSize,
                 const std::string& compressedMd5);

        bool IsValid() const;

        const std::string& GetUuid() const;

        FileContentType GetContentType() const;

        uint64_t GetUncompressedSize() const;

        CompressionType GetCompressionType() const;

        uint64_t GetCompressedSize() const;

        const std::string& GetCompressedMD5() const;

        const std::string& GetUncompressedMD5() const;
        
    };

}