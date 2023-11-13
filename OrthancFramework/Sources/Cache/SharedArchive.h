
#pragma once

#if !defined(ORTHANC_SANDBOXED)
#  error The Macro ORTHANC_SANDBOXED must be defined
#endif

#if ORTHANC_SANDBOXED == 1
#  error The class SharedArchive cannot be used in a sandboxed environment
#endif

#include "LeastRecentlyUsedIndex.h"
#include "../IDynamicObject.h"

#include <map>
#include <boost/thread.hpp>

namespace Orthanc
{
    class ORTHANC_PUBLIC SharedArchive : public boost::noncopyable
    {
    private:
        typedef std::map<std::string, IDynamicObject*>  Archive;

        size_t                                          maxSize_;
        boost::mutex                                    mutex_;
        Archive                                         archive_;
        LeastRecentlyUsedIndex<std::string>             lru_;

        void RemoveInternal(const std::string& id);

    public:
        class ORTHANC_PUBLIC Accessor : public boost::noncopyable
        {
        private:
            boost::mutex::scope_lock   lock_;
            IDynamicObject*            item_;

        public:
            Accessor(SharedArchive& that, const std::string& id);

            bool IsValid() const;

            IDynamicObject& GetItem() const;
        };

        explicit SharedArchive(size_t maxSize);

        ~SharedArchive();

        std::string Add(IDynamicObject* obj); // Takes the ownership

        void Remove(const std::string& id);

        void List(std::list<std::string>& items);
    };

}