#pragma once

#include "OrthancFramework.h"

#include <boost/noncopyable.hpp>

namespace Orthanc
{
    /**
     * This class should be the ancestor to any class whose type is
     * determined at the runtime, and that can be dynamically allocated.
     * Being a child of IDynamicObject only implies the existence of a
     * virtual destructor.
     **/
    class ORTHANC_PUBLIC IDynamicObject : public boost::noncopyable
    {
    public:
        virtual ~IDynamicObject() {}
    };

    /**
     * This class is a simple implementation of a IDynamicObject that
     * stores a single typed value.
     */
    template <typename T>
    class SingleValueObject : public IDynamicObject
    {
    private:
        T value_;

    public:
        SingleValueObject(const T& value)
            : value_(value)
        {
        }

        const T& GetValue() const
        {
            return value_;
        }

        void SetValue(const T& value)
        {
            value_ = value;
        }
    };

}