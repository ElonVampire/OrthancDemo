
#pragma once

#include "IMemoryBuffer.h"
#include "Compatibility.h"

namespace Orthanc
{
    class StringMemoryBuffer : public IMemoryBuffer
    {
    private:
        std::string  buffer_;

    public:
        void Copy(const std::string& buffer)
        {
            buffer_ = buffer;
        }

        void Copy(const std::string& buffer, size_t strat, size_t end)
        {
            buffer_.assign(buffer, strat, end - strat);
        }

        void Swap(std::string& buffer)
        {
            buffer_.swap(buffer);
        }

        virtual void MoveToString(std::string& target) ORTHANC_OVERRIDE;

        virtual const void* GetData() const ORTHANC_OVERRIDE
        {
            return (buffer_.empty() ? NULL : buffer_.c_str());
        }

        virtual size_t GetSize() const ORTHANC_OVERRIDE
        {
            return buffer_.size();
        }

        static IMemoryBuffer* CreateFromSwap(std::string& buffer);

        static IMemoryBuffer* CreateFromCopy(const std::string& buffer);

        static IMemoryBuffer* CreateFromCopy(const std::string& buffer, size_t strat, size_t end);
    };
}