#include "../PrecompiledHeaders.h"
#include "MemoryObjectCache.h"

#include "../Compatibility.h"

namespace Orthanc
{
    class MemoryObjectCache::Item : public boost::noncopyable
    {
    private:
        ICacheable*                 value_;
        boost::posix_time::ptime    time_;

    public:
        explicit Item(ICacheable* value)
          : value_(value)
          , time_(boost::posix_time::second_clock::local_time())
        {
            if(value_ == NULL)
            {
                throw OrthancException(ErrorCode_NullPointer);
            }
        }

        ~Item()
        {
            assert(value_ != NULL);
            delete value_;
        }

        ICacheable& GetValue() const
        {
            assert(value_ != NULL);
            return *value_;
        }

        const boost::posix_time::ptime& GetTime() const
        {
            return time_;
        }
    };

    void MemoryObjectCache::Recycle(size_t targetSize)
    {
        // WARNING: "cacheMutex_" must be locked
        while(currentSize_ > targetSize)
        {
            assert(!content_.IsEmpty());

            Item* item = NULL;
            content_.RemoveOldest(item);

            assert(item != NULL);
            const size_t size = item->GetValue().GetMemoryUsage();
            delete item;

            assert(currentSize_ >= size);
            currentSize_ -= size;
        }
    }
}