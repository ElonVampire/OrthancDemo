#include "PrecompiledHeaders.h"
#include "ChunkedBuffer.h"

#include <cassert>
#include <string.h>

namespace Orthanc
{
    void ChunkedBuffer::Clear()
    {
        numBytes_ = 0;
        pendingPos_ = 0;
        for(Chunks::iterator it = chunks_.begin(); it != chunks_.end(); ++it)
        {
            delete *it;
        }
    }

    void ChunkedBuffer::AddChunkInternal(const void* chunkData, size_t chunkSize)
    {
        if(chunkSize == 0)
        {
            return;
        }
        else
        {
            assert(chunkData != NULL);
            chunks_.push_back(new std::string(reinterpret_cast<const char*>(chunkData), chunkSize));
            numBytes_ += chunkSize;
        }
    }


    
}