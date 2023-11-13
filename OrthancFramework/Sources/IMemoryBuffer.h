
#pragma once

#include <string>
#include <boost/noncopyable.hpp>

namespace Orthanc
{
    /**
     * This class abstracts a memory buffer and its memory uallocation
     * function.
     */
    class IMemoryBuffer : public boost::noncopyable
    {
    public:
      virtual ~IMemoryBuffer() {}

      // The content of the memory buffer will emptied after this call
      virtual void MoveToString(std::string& target) = 0;

      virtual const void* GetData() const = 0;

      virtual size_t GetSize() const = 0;
}