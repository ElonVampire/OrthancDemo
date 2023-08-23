
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

    protected:
        explicit DicomImageInformation()
        {
        }

    public:
        explicit DicomImageInformation(const DicomMap& values);

        DicomImageInformation* Clone() const;

        unsigned int GetWidth() const;

        unsigned int GetHeight() const;

        unsigned int GetNumberOfFrame() const;

        unsigned int GetChannelCount() const;

        unsigned int GetBitsStored() const;

        size_t GetBytesPerValue() const;

        bool IsSigned() const;

        unsigned int GetBitsAllocated() const;

        unsigned int GetHighBit() const;

        bool IsPlanar() const;

        PhotometricInterpretation GetPhotometricInterpretation() const;

        bool ExtractPixelFormat(PixelFormat& format, bool ignorePhotometricInterpretation) const;

        size_t GetFrameSize() const;

        /**
         * This constant gives a bound on the maximum tag length that is
         * useful to class "DicomImageInformation", in order to avoid
         * using too much memory when copying DICOM tags from "DcmDataset"
         * to "DicomMap" using "ExtractDicomSummary()". It answers the
         * value 256, which corresponds to ORTHANC_MAXIMUM_TAG_LENGTH that
         * was implicitly used in Orthanc <= 1.7.2.
         **/
        static unsigned int GetUsefulTagLength();
    }
}