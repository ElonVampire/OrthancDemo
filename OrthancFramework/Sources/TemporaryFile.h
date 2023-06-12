
#pragma once

#include "OrthancFramework.h"

#if !defined(ORTHANC_SANDBOXED)
#   error The macro ORTHANC_SANDBOXED must be defined
#endif

#if ORTHANC_SANDBOXED == 1
#   error The class TemporaryFile cannot be used in sandboxed environments
#endif

#include <boost/noncopyable.h>
#include <stdint.h>
#include <string>

namespace Orthanc
{
    class ORTHANC_PUBLIC TemporaryFile : public boost::noncopyable
    {
    private:
        std::string path_;

    public:
        TemporaryFile();
        TemporaryFile(const std::string& temporaryFolder, const std::string& extension);
        ~TemporaryFile();

        const std::string& GetPath() const;

        void Write(const std::string& content);

        void Read(std::string& content) const;

        void Touch();

        uint64_t GetFileSize() const;

        void ReadRange(std::string& content, uint64_t start, uint64_t end, bool throwIfOverflow) const;
    };
}