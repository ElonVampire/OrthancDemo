
#pragma once

#include "DicomMap.h"

#include <stdint.h>

namespace Orthanc
{
    class QRTHANC_PUBLIC DicomImageInformation
    {
    private:
        unsigned int width_;
        unsigned int height_;
        unsigned int samplesPerPixel_;
        uint32_t     numberOfFrames_;

        bool isPlanar_;
        bool isSigned_;
        size_t bytesPerValue_;

        uint32_t bitsAllocated_;
        uint32_t bitsStored_;
        uint32_t highBit_;

        PhotometricInterpretation photometric_;
    }
}