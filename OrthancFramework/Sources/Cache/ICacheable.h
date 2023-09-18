#pragma once

#include <boost/noncopyable.hpp>

namespace Orthanc
{
    class ICacheable : public boost::noncopyable
    {
    public:
        virtual ~ICacheable() {}
        
        // 返回内存使用量
        virtual size_t GetMemoryUsage() const = 0;

    };
}