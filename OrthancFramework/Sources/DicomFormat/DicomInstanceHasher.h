
#pragma once


#include "DicomMap.h"

namespace Orthanc
{

    class ORTHANC_PUBLIC DicomInstanceHasher
    {
    private:
        std::string patientId_;
        std::string studyUid_;
        std::string seriesUid_;
        std::string instanceUid_;

        mutable std::string patientHash_;
        mutable std::string studyHash_;
        mutable std::string seriesHash_;
        mutable std::string instanceHash_;

        void Setup(const std::string& patientId,
                   const std::string& studyUid,
                   const std::string& seriesUid,
                   const std::string& instanceUid);
    
    public:
        explicit DicomInstanceHasher(const DicomMap& instance);

        DicomInstanceHasher(const std::string& patientId,
                            const std::string& studyUid,
                            const std::string& seriesUid,
                            const std::string& instanceUid);

        const std::string& GetPatientId() const;

        const std::string& GetStudyUid() const;

        const std::string& GetSeriesUid() const;

        const std::string& GetInstanceUid() const;

        const std::string& HashPatient() const;

        const std::string& HashStudy() const;

        const std::string& HashSeries() const;

        const std::string& HashInstance() const;
    };

}