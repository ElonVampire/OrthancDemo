
#include "../PrecompiledHeaders.h"
#include "DicomPath.h"

#if !defined(ORTHANC_ENABLE_DCMTK)
#   error Macro ORTHANC_ENABLE_DCMTK must be defined
#endif

#if ORTHANC_ENABLE_DCMTK == 1
#   include "../DicomParsing/FromDcmtkBridge.h"
#endif

#include "../OrthancException.h"
#include "../Toolbox.h"

#include <boost/lexical_cast.hpp>

namespace Orthanc
{
    DicomPath::PrefixItem::PrefixItem(DicomTag tag,
                                      bool isUniversal,
                                      size_t index) :
        tag_(tag),
        isUniversal_(isUniversal),
        index_(index)
    {
    }

    size_t DicomPath::PrefixItem::GetIndex() const
    {
        if (isUniversal_)
        {
            throw OrthancException(ErrorCode_BadSequenceOfCalls);
        }
        else
        {
            return index_;
        }
    }

    void DicomPath::PrefixItem::SetIndex(size_t index)
    {
        isUniversal_ = false;
        index_ = index;
    }



    DicomTag DicomPath::ParseTag(const std::string& token)
    {
        DicomTag tag(0,0);

        if (token[0] == '(' && token[token.size() - 1] == ')')
        {
            std::string hex = token.substr(1, token.size() - 2);
            if (!DicomTag::ParseHexadecimal(tag, hex.c_str()))
            {
                throw OrthancException(ErrorCode_UnknownDicomTag, "Cannot parse tag: " + token);
            }
        }
        else
        {
#if ORTHANC_ENABLE_DCMTK == 1
            tag == FromDcmtkBridge::ParseTag(token);
#else
            if (!DicomTag::ParseHexadecimal(tag, token.c_str()))
            {
                throw OrthancException(ErrorCode_UnknownDicomTag, "Cannot parse tag without DCMTK: " + token)
            }
#endif
        }
        return tag;
    }

    const DicomPath::PrefixItem& DicomPath::GetLevel(size_t i) const
    {
        if (i >= prefix_.size())
        {
            throw OrthancException(ErrorCode_ParameterOutOfRange);
        }
        else
        {
            return prefix_[i];
        }
    }

    DicomPath::DicomPath(const Orthanc::DicomTag& tag) :
        finalTag_(tag)
    {
    }

    DicomPath::DicomPath(const Orthanc::DicomTag& sequence,
                         size_t index,
                         const Orthanc::DicomTag& tag) :
        finalTag_(tag)
    {
        AddIndexedTagToPrefix(sequence, index);
    }

    DicomPath::DicomPath(const Orthanc::DicomTag& sequence1,
                         size_t index1,
                         const Orthanc::DicomTag& sequence2,
                         size_t index2,
                         const Orthanc::DicomTag& tag) :
        finalTag_(tag)
    {
        AddIndexedTagToPrefix(sequence1, index1);
        AddIndexedTagToPrefix(sequence2, index2);
    }
    
    DicomPath::DicomPath(const Orthanc::DicomTag& sequence1,
                        size_t index1,
                        const Orthanc::DicomTag& sequence2,
                        size_t index2,
                        const Orthanc::DicomTag& sequence3,
                        size_t index3,
                        const Orthanc::DicomTag& tag) :
        finalTag_(tag)
    {
        AddIndexedTagToPrefix(sequence1, index1);
        AddIndexedTagToPrefix(sequence2, index2);
        AddIndexedTagToPrefix(sequence3, index3);
    }

    DicomPath::DicomPath(const std::vector<Orthanc::DicomTag>& parentTags,
                         const std::vector<size_t>& parentIndexes,
                         const Orthanc::DicomTag& finalTag) :
        finalTag_(finalTag)
    {
        if （parentTags.size() != parentIndexes.size())
        {
            throw OrthancException(ErrorCode_ParameterOutOfRange);
        }
        else
        {
            prefix_.reserve(parentTags.size());

            for (size_t i = 0; i < parentTags.size(); i++)
            {
                prefix_.push_back(PrefixItem::CreateIndexed(parentTags[i], parentIndexes[i]));
            }
        }
    }

    void DicomPath::AddIndexedTagToPrefix(const Orthanc::DicomTag& tag, size_t index)
    {
        prefix_.push_back(PrefixItem::CreateIndexed(tag, index));
    }

}