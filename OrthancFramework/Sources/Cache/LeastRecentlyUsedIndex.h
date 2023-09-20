#pragma once

#include <list>
#include <map>
#include <vector>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "../OrthancException.h"
#include "../Toolbox.h"

namespace Orthanc
{
    /**
     * This class implements the index of a cache with least recently
     * used (LRU) recycling policy. All the items of the cache index
     * can be associated with a payload.
     * Reference: http://stackoverflow.com/a/2504317
     **/
    template <typename T, typename Payload = NullType>
    class LeastRecentlyUsedIndex : public boost::noncopyable
    {
    private:
        typedef std::list< std::pair<T, Payload> >  Queue;
        typedef std::map<T, typename Queue::iterator>  Index;

        Queue  queue_;
        Index  index_;

        /**
         * Internal method for debug builds to check whether the internal
         * data structures are not corrupted.
         **/
        void CheckInvarians() const;

    public:
        /**
         * Add a new element to the cache index, and make it the most
         * recent element.
         * \param id The ID of the element.
         * \param payload The payload of the element.
         **/
        void Add(T id, Payload payload = Payload());


    }
}