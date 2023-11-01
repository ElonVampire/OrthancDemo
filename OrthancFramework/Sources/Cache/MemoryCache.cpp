#include "../PrecompiledHeaders.h"
#include "MemoryCache.h"

#include "../Logging.h"

namespace Orthanc
{
    namespace Deprecated
    {
        MemoryCache::Page& MemoryCache::Load(const std::string& id)
        {
            // Reuse the cache entry if it already exists
            Page* p = NULL;
            if (index_.Contains(id, p))
            {
                LOG(TRACE) << "Reusing a cache page";
                assert(p != NULL);
                index_.MakeMostRecent(id);
                return *p;
            }

            //The id is not in the cache yet. make some room if the cache is full
            if(index_.GetSize() == cacheSize_)
            {
                LOG(TRACE) << "Dropping the oldest cache page";
                index_.RemoveOldest(p);
                delete p;
            }

            //Create a new cache page
            std::unique_ptr<Page> result(new Page);
            result->id_ = id;
            result->content_.reset(provider_.Provide(id));

            //Add the newly create page to the cache
            LOG(TRACE) << "Registering new data in a cache page";
            p = result.release();
            index_.Add(id, p);
            return *p;
        }

        MemoryCache::MemoryCache(ICachePageProvider& provider, size_t cacheSize):
          provider_(provider),
          cacheSize_(cacheSize)
        {
        }

        void MemoryCache::~MemoryCache()
        {
            while(!index_.IsEmpty())
            {
                Page* element = NULL;
                index_.RemoveOldest(element);
                assert(element != NULL);
                delete element;
            }
        }

        void MemoryCache::Invalidate(const std::string& id)
        {
            Page* p = NULL;
            if(index_.Contains(id, p))
            {
                LOG(TRACE) << "Invalidating the cache page";
                assert(p != NULL);
                delete p;
                index_.invalidate(id);
            }
        }

        IDynamicObject& MemoryCache::Access(const std::string& id)
        {
            return *Load(id).content_;
        }
    }
}