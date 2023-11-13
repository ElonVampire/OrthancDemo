
#include "../PrecompiledHeaders.h"
#include "FileInfo.h"

#include "../OrthancException.h"

namespace Orthanc
{
    FileInfo::FileInfo() :
        valid_(false),
        contentType_(FileContentType_Unknown),
        uncompressedSize_(0),
        compressionType_(CompressionType_None),
        compressedSize_(0)
    {
    }

    FileInfo::FileInfo(const std::string& uuid,
                        FileContentType contentType,
                        uint64_t size,
                        const std::string& md5) :
        valid_(true),
        uuid_(uuid),
        contentType_(contentType),
        uncompressedSize_(size),
        uncompressedMD5_(md5),
        compressionType_(CompressionType_None),
        compressedSize_(size),
        compressedMD5_(md5)
    {
    }


    FileInfo::FileInfo(const std::string& uuid,
                        FileContentType contentType,
                        uint64_t uncompressedSize,
                        const std::string& uncompressedMD5,
                        CompressionType compressionType,
                        uint64_t compressedSize,
                        const std::string& compressedMD5) :
        valid_(true),
        uuid_(uuid),
        contentType_(contentType),
        uncompressedSize_(uncompressedSize),
        uncompressedMD5_(uncompressedMD5),
        compressionType_(compressionType),
        compressedSize_(compressedSize),
        compressedMD5_(compressedMD5)
    {
    }

    bool FileInfo::IsValid() const
    {
        return valid_;
    }

    const std::string& FileInfo::GetUuid() const
    {
        if(valid_)
        {
            return uuid_;
        }
        else
        {
            throw OrthancException(ErrorCode_BadSequenceOfCalls);
        }
    }

    FileContentType FileInfo::GetContentType() const
    {
        if(valid_)
        {
            return contentType_;
        }
        else
        {
            throw OrthancException(ErrorCode_BadSequenceOfCalls);
        }
    }

    uint64_t FileInfo::GetUncompressedSize() const
    {
        if(valid_)
        {
            return uncompressedSize_;
        }
        else
        {
            throw OrthancException(ErrorCode_BadSequenceOfCalls);
        }
    }

    CompressionType FileInfo::GetCompressionType() const
    {
        if(valid_)
        {
            return compressionType_;
        }
        else
        {
            throw OrthancException(ErrorCode_BadSequenceOfCalls);
        }
    }

    

}