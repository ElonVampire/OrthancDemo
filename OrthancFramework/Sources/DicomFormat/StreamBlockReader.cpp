
#include "../PrecompiledHeaders.h"
#include "StreamBlockReader.h"

#include "../OrthancException.h"

namespace Orthanc
{
    StreamBlockReader::StreamBlockReader(std::istream& stream) :
        stream_(stream),
        blockPos_(0),
        processedBytes_(0)
    {
    }

    void StreamBlockReader::Schedule(size_t blockSize)
    {
        if (!block_.empty())
        {
            throw OrthancException(ErrorCode_BadSequenceOfCalls);
        }
        else
        {
            block_.resize(blockSize);
            blockPos_ = 0;
        }
    }

    bool StreamBlockReader::Read(std::string& block)
    {
        if (block_.empty())
        {
            if (blockPos_ != 0)
            {
                throw OrthancException(ErrorCode_BadSequenceOfCalls);
            }
            
            block.clear();
            return true;
        }
        else
        {
            while (blockPos_ < block_.size())
            {
                
            }
        }
    }
}