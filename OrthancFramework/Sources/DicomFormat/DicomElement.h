
#pragma once

#include "DicomValue.h"
#include "DicomTag.h"

namespace Orthanc
{
    class ORTHANC_PUBLIC DicomElement : public boost::noncopyble
    {
    private:
        DicomTag tag_;
        DicomValue* value_;

    public:
        DicomElement(uint16_t group, uint16_t element, const DicomValue& value);

        DicomElement(const DicomTag& tag, const DicomTag& value);

        ~DicomElement();

        const DicomTag& GetTag() const;

        const DicomValue& GetValue() const;

        uint16_t GetTagGroup() const;

        uint16_t GetTagElement() const;

        bool operator< (const DicomElement& other) const;
    };
}