#pragma once

#include <string>
#include "../IDynamicObject.h"

namespace Orthanc
{
    namespace Deprecated
    {
        class ICachePageProvider
        {
        public:
            virtual ~ICachePageProvider() {}

            virtual IDynamicObject* Provide(const std::string& id) = 0;
        }
    }
}