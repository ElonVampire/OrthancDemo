
#include "../PrecompiledHeaders.h"
#include "MemoryStringCache.h"

namespace Orthanc
{
    class MemoryStringCache::StringValue : public ICacheable
    {
    private:
        std::string content_;

    public:
        explicit StringValue(const std::string& content)
          : content_(content)
        {
        }

        explicit StringValue(const char* buffer, size_t size)
          : content_(buffer, size)
        {
        }

        const std::string& GetContent() const
        {
            return content_;
        }

        virtual size_t GetMemoryUsage() const
        {
            return content_.size();
        }
    };

    size_t MemoryStringCache::GetMaximumSize()
    {
        return cache_.GetMaximumSize();
    }

    void MemoryStringCache::SetMaximumSize(size_t size)
    {
        cache_.SetMaximumSize(size);
    }

    void MemoryStringCache::Add(const std::string& key, const std::string& value)
    {
        cache_.Acquire(key, new StringValue(value));
    }

    void MemoryStringCache::Add(const std::string& key, const void* buffer, size_t size)
    {
        cache_.Acquire(key, new StringValue(reinterpret_cast<const char*>(buffer), size));
    }

    void MemoryStringCache::Invalidate(const std::string& key)
    {
        cache_.Invalidate(key);
    }

    bool MemoryStringCache::Fetch(std::string& value, const std::string& key)
    {
        MemoryObjectCache::Accessor reader(cache_, key, false /* multiple readers are allowed */);

        if(reader.IsValid())
        {
            value = dynamic_cast<StringValue*>(reader.GetObject())->GetContent();
            return true;
        }
        else
        {
            return false;
        }
    }

}