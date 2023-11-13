
#include "../PrecompiledHeaders.h"
#include "SharedArchive.h"

#include "../Toolbox.h"

namespace Orthanc
{
    void SharedArchive::RemoveInternal(const std::string& id)
    {
        Archive::iterator it = archive_.find(id);

        if (it != archives_.end())
        {
            delete it->second;
            archive_.erase(it);

            lru_.Invalidate(id);
        }
    }

    SharedArchive::Accessor::Accessor(SharedArchive& that, const std::string& id)
      : lock_(that.mutex_)
    {
        Archive::iterator it = that.archive_.find(id);

        if (it == that.archive_.end())
        {
            item_ = NULL;
        }
        else
        {
            that.lru_.MakeMostRecent(id);
            item_ = it->second;
        }
    }

    bool SharedArchive::Accessor::IsValid() const
    {
        return item_ != NULL;
    }

    IDynamicObject& SharedArchive::Accessor::GetItem() const
    {
        if (item_ == NULL)
        {
            // "IsValid()" should have been called
            throw OrthancException(ErrorCode_BadSequenceOfCalls);
        }
        else
        {
            return *item_;
        }
    }  


    SharedArchive::SharedArchive(size_t maxSize) : 
        maxSize_(maxSize)
    {
        if (maxSize == 0)
        {
        throw OrthancException(ErrorCode_ParameterOutOfRange);
        }
    }


    SharedArchive::~SharedArchive()
    {
        for (Archive::iterator it = archive_.begin();
            it != archive_.end(); ++it)
        {
        delete it->second;
        }
    }


    std::string SharedArchive::Add(IDynamicObject* obj)
    {
        boost::mutex::scoped_lock lock(mutex_);

        if (archive_.size() == maxSize_)
        {
        // The quota has been reached, remove the oldest element
        RemoveInternal(lru_.GetOldest());
        }

        std::string id = Toolbox::GenerateUuid();
        RemoveInternal(id);  // Should never be useful because of UUID

        archive_[id] = obj;
        lru_.Add(id);

        return id;
    }


    void SharedArchive::Remove(const std::string& id)
    {
        boost::mutex::scoped_lock lock(mutex_);
        RemoveInternal(id);      
    }


    void SharedArchive::List(std::list<std::string>& items)
    {
        items.clear();

        {
            boost::mutex::scoped_lock lock(mutex_);

            for (Archive::const_iterator it = archive_.begin();
                it != archive_.end(); ++it)
            {
                items.push_back(it->first);
            }
        }
    }
}