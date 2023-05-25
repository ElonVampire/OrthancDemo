
#pragma once

#include "OrthancFramework.h"

#include <boost/noncopyable.hpp>
#include <list>
#include <string>

namespace Orthanc
{
    class ORTHANC_PUBLIC ChunkedBuffer : public boost::noncopyable
    {
    private:
        typedef std::list<std::string*> Chunks;

        size_t      numBytes;
        Chunks      chunks_;
        std::string pendingBuffer;  // Buffer to speed up if adding many small chunks
        size_t      pendingPos_;

        void    Clear();
        void    AddChunkInternal(const void* chunkData, size_t chunkSize);
        void    FlushPendingBuffer();

    public:
        ChunkedBuffer();
        ~ChunkedBuffer();

        size_t  GetNumBytes() const;
        void    SetPendingBufferSize(size_t size);
        size_t  GetPendingBufferSize() const;

        void    AddChunk(const void* chunkData, size_t chunkSize);
        void    AddChunk(const std::string& chunk);
        void    AddChunk(const std::string::const_iterator& begin, const std::string::const_iterator& end);
        void    Flatten(std::string& result);
    };
}