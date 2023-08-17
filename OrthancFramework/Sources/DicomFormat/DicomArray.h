
#pragma once

#include "DicomElement.h"
#include "DicomMap.h"

#include <vector>

namespace Orthanc
{
    class ORTHANC_PUBLIC DicomArray : public boost::noncopyable
    {
    private:
        typedef std::vector<DicomElement*> Elements;
        Elements elements_;

    public:
        explicit DicomArray(const DicomMap& map);

        ~DicomArray();

        size_t GetSize() const;

        const DicomElement& GetElement(size_t i) const;

        void GetTags(std::set<DicomTag>& tags) const;

        void Print(FILE* fp) const;     // for debugging only
    }
}