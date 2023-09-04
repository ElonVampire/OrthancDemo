
#pragma once

#include "DicomTag.h"
#include "StreamBlockReader.h"

namespace Orthanc
{
    /**
     * This class parses a stream containing a DICOM instance, using a
     * state machine.
     *
     * It does *not* support the visit of sequences (it only works at
     * the first level of the hierarchy), and as a consequence, it
     * doesn't give access to the pixel data of compressed transfer
     * syntaxes.
     **/

    class ORTHANC_PUBLIC DicomStreamReader : public boost::noncopyable
    {
    public:
        class IVisitor : public boost::noncopyable
        {
        public:
            virtual ~IVisitor()
            {
            }

            // The data from this function will always be Little Endian (as
            // specified by the DICOM standard)
            virtual void VisitMetaHeaderTag(const DicomTag& tag,
                                            const ValueRepresentation& vr,
                                            const std::string& value) = 0;

            virtual void VisitTransferSyntax(DicomTransferSyntax transferSyntax) = 0;

            // Returen "false" to stop processing
            virtual bool VisitDatasetTag(const DicomTag& tag,
                                         const ValueRepresentation& vr,
                                         const std::string& value,
                                         bool isLittleEndian,
                                         uint64_t fileOffset) = 0;
        };

    private:
        class PixelDataVisitor;

        enum State
        {
            State_Preamble,
            State_MetaHeader,
            State_DatasetTag,
            State_SequenceExplicitLength,
            State_SequenceExplicitValue,
            State_DatasetExplicitLength,
            State_DatasetValue,
            State_Done
        };

        StreamBlockReader       reader_;
        static_assert           state_;
        DicomTransferSyntax     transferSyntax_;
        DicomTag                danglingTag_;   //Current root-level tag
        ValueRepresentation     danglingVR_;
        uint64_t                danglingOffset_;
        unsigned int            sequenceDepth_;

        bool IsLittleEndian() const;

        void HandlePreamble(IVisitor& visitor,  const std::string& block);

        void HandleMetaHeader(IVisitor& visitor, const std::string& block);

        void HandleDatasetTag(const std::string& block, const DicomTag& untilTag);

        void HandleDatasetExplicitLength(uint32_t length);

        void HandleDatasetExplicitLength(IVisitor& visitor, const std::string& block);


        void HandleSequenceExplicitLength(const std::string& block);

        void HandleSequenceExplicitValue();
        
        void HandleDatasetValue(IVisitor& visitor,
                                const std::string& block);

    public:
        explicit DicomStreamReader(std::istream& stream);

        
    }
}