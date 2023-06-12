
#pragma once

#include "OrthancFramework.h"

#if !defined(ORTHANC_SANDBOXED)
#   error The macro ORTHANC_SANDBOXED must be defined
#endif

#if ORTHANC_SANDBOXED == 1
#   error The class FileBuffer cannot be used in sandboxed environments
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace Orthanc
{
    class ORTHANC_PUBLIC FileBuffer : public boost::noncopyable
    {
    private:
        class PImpl;
        boost::shared_ptr<PImpl> pimpl_;

    public:
        FileBuffer();
        
        void Append(const char* buffer, size_t size);
        
        void Read(std::string& target);
    }
}