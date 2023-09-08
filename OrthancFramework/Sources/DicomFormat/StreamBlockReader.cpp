
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
                /**
                 * WARNING: Do NOT use "stream_.readsome()", as it does not
                 * work properly on non-buffered stream (which is the case in
                 * "DicomStreamReader::LookupPixelDataOffset()" for buffers)
                 **/

                size_t remainingBytes = block_.size() - blockPos_;
                stream_.read(&block_[blockPos_], remainingBytes);

                std::streamsize r = stream_.gcount();
                if (r == 0)
                {
                    return false;
                }
                else
                {
                    blockPos_ += r;
                }
            }

            processedBytes_ += block_.size();

            block.swap(block_);
            block_.clear();
            return true;
        }
    }

    uint64_t StreamBlockReader::GetProcessedBytes() const
    {
        return processedBytes_;
    }
}