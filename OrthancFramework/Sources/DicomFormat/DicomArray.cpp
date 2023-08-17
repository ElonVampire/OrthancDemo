
#include "../PrecompiledHeaders.h"
#include "DicomArray.h"

#include "../OrthancException.h"

#include <stdio.h>

namespace Orthanc
{
    DicomArray::DicomArray(const DicomMap& map)
    {
        elements_.reserve(map.content_.size());

        for (DicomMap::Content::const_iterator it = map.content_.begin(); it != map.content_.end(); ++it)
        {
            elements_.push_back(new DicomElement(it->first, *it->second));
        }
    }

    DicomArray::~DicomArray()
    {
        for (size_t i = 0; i < elements_.size(); i++)
        {
            delete elements_[i];
        }
    }

    size_t DicomArray::GetSize() const
    {
        return elements_.size();
    }

    const DicomElement& DicomArray::GetElement(size_t i) const
    {
        if (i >= elements_.size())
        {
            throw OrthancException(ErrorCode_ParameterOutOfRange);
        }
        else
        {
            return *elements_[i]
        }
    }

    void DicomArray::GetTags(std::set<DicomTag>& tags) const
    {
        tags.clear();

        for (size_t i = 0; i < elements_.size(); i++)
        {
            tags.insert(elements_[i]->GetTag());
        }
    }

    void DicomArray::Print(FILE* fp) const
    {
        for (size_t i = 0; i < elements_.size(); i++)
        {
            DicomTag t = elements_[i]->GetTag();
            const DicomValue& v = elements_[i]->GetValue();
            std::string  s = v.isNull() ? "(null)" : v.GetContent();
            printf("9x%04x 0x%04x [%s]\n", t.GetGroup(), t.GetElement(), s.c_str());
        }
    }
}