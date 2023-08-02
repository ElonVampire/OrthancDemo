
#include "../PrecompiledHeaders.h"
#include "DicomElement.h"

namespace Orthanc
{
    DicomElement::DicomElement(uint16_t group, uint16_t element, const DicomValue& value) :
        tag_(group, element),
        value_(value.Clone())
    {
    }

    DicomElement::DicomElement(const DicomTag& tag, const DicomValue& value) :
        tag_(tag),
        value_(value)
    {
    }

    DicomElement::~DicomElement()
    {
        delete value_;
    }

    const DicomTag& DicomElement::GetTag() const
    {
        return tag_;
    }

    const DicomValue& DicomElement::GetValue() const
    {
        return *value_;
    }

    uint16_t DicomElement::GetTagGroup() const
    {
        return tag_.GetGroup();
    }

    uint16_t DicomElement::GetTagElement() const
    {
        return tag_.GetElement();
    }

    bool DicomElement::operator< (const DicomElement& other) const
    {
        return GetTag() < other.GetTag();
    }
}