

#pragma once

#include "DicomMap.h"
#include "DicomImageInformation.h"

#include <stdint.h>

namespace Orthanc
{
    class DicomIntegerPixelAccessor
    {
    private:
        DicomImageInformation information_;

        uint32_t signMask_;
        uint32_t mask_;

        const void* pixelData_;
        size_t size_;
        unsigned int frame_;
        size_t frameOffset_;
        size_t rowOffset_;

    public:
        DicomIntegerPixelAccessor(const DicomMap& values,
                                  const void* pixelData,
                                  size_t size);
        
        const DicomImageInformation GetInformation() const
        {
            return information_;
        }

        
        unsigned int GetCurrentFrame() const
        {
            return frame_;
        }

        void SetCurrentFrame(unsigned int frame);

        void GetExtremeValues(int32_t& min, 
                            int32_t& max) const;

        int32_t GetValue(unsigned int x,
                        unsigned int y,
                        unsigned int channel) const;

        const void* GetPixelData() const
        {
            return pixelData_;
        }

        size_t GetSize() const
        {
            return size_;
        }
    };
}