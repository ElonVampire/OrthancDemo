
#pragma once

#include "MemoryObjectCache.h"

namespace Orthanc
{
    /**
     * Facade object around "MemoryObjectCache" that caches a dictionary
     * of strings, using the "fetch/add" paradigm of memcached.
     * 
     * Note: this class is thread safe
     **/
    class ORTHANC_PUBLIC MemoryStringCache : public boost::noncopyable
    {
    private:
        class StringValue;
        
        MomeryObjectCache cache_;

    public:
        size_t GetMaximumSize();

        void SetMaximumSize(size_t size);

        void Add(const std::string& key, const std::string& value);

        void Add(const std::string& key, const void* buffer, size_t size);

        void Invalidate(const std::string& key);

        bool Fetch(std::string& value, const std::string& key);
    };
}