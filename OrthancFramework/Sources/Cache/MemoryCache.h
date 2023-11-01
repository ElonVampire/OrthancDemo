#pragma once

#include  "../Compatibility.h"
#include  "ICahcePageProvider.h"
#include "LeastRecentlyUsedIndex.h"
#include <memory>

namespace Orthanc
{
    namespace Deprecated
    {
        /**
         * WARNING: This class is NOT thread-safe.      * 
         */
        class ORTHANC_PUBLIC MemoryCache : public boost::noncopyable
        {
        private:
            struct Page
            {
                std::string id_;
                std::unique_ptr<IDynamicObject> content_;
            };

            ICachePageProvider& provider_;
            size_t cacheSize_;
            LeastRecentlyUsedIndex<std::string, Page*> index_;

            Page& Load(const std::string& id);

        public:
            MemoryCache(ICachePageProvider& provider, size_t cacheSize);
            ~MemoryCache();

            IDynamicObject& Access(const std::string& id);

            void Invalidate(const std::string& id);

        };
    }
}