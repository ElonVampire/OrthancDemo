
#pragma once

#include "../IMemoryBuffer.h"
#include "../Enumerations.h"

#include <stdint.h>
#include <string>


namespace Orthanc
{
    class IStorageArea : public boost::noncopyable
    {
    public:
        virtual ~IStorageArea() {}

        virtual void Create(const std::string& uuid,
                            const void* content,
                            size_t size,
                            FileContentType type) = 0;

        virtual IMemoryBuffer* Read(const std::string& uuid,
                                    FileContentType* type) = 0;

        virtual IMemoryBuffer* ReadRange(const std::string& uuid,
                                         FileContentType* type,
                                         uint64_t start,
                                         uint64_t end) = 0;

        virtual bool HasReadRange() const = 0;

        virtual void Remove(const std::string& uuid,
                            FileContentType type) = 0;
    };
}