
#include "PrecompiledHeaders.h"
#include "StringMemoryBuffer.h"

namespace Orthanc
{
    void StringMemoryBuffer::MoveToString(std::string& target)
    {
        buffer_.swap(target);
        buffer_.clear();
    }

    IMemoryBuffer* StringMemoryBuffer::CreateFromSwap(std::string& buffer)
    {
        std::unique_ptr<StringMemoryBuffer> result(new StringMemoryBuffer);
        result->Swap(buffer);
        return result.release();
    }

    IMemoryBuffer* StringMemoryBuffer::CreateFromCopy(const std::string& buffer)
    {
        std::unique_ptr<StringMemoryBuffer> result(new StringMemoryBuffer);
        result->Copy(buffer);
        return result.release();
    }

    IMemoryBuffer* StringMemoryBuffer::CreateFromCopy(const std::string& buffer, size_t start, size_t end)
    {
        std::unique_ptr<StringMemoryBuffer> result(new StringMemoryBuffer);
        result->Copy(buffer, start, end);
        return result.release();
    }
}