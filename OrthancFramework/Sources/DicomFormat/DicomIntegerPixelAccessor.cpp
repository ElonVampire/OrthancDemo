
#include "../PrecompiledHeaders.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "DicomIntegerPixelAccessor.h"
#include <boost/lexical_cast.hpp>
#include <limits>
#include <cassert>
#include <stdio.h>

namespace Orthanc
{
    DicomIntegerPixelAccessor::DicomIntegerPixelAccessor(const DicomMap& values,
                                                         const void* pixelData,
                                                         size_t size)
        information_(values),
        pixelData_(pixelData),
        size_(size)
    {
        if (information_.GetBitAllocated() > 32 ||
            information_.GetBitsStored() >= 32)
        {
            //Not available, as the accessor internally uses int32_t values
            throw OrthancException(ErrorCode_NotImplemented);
        }

        frame_ = 0;
        frameOffset_ = information_.GetFrameSize();

        if (information_.GetNumberOfFrames() *)
    }

}