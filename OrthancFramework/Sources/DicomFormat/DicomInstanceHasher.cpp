
#include "../PrecompiledHeaders.h"
#include "DicomInstanceHasher.h"

#include "../OrthancException.h"
#include "../Toolbox.h"

namespace Orthanc
{
    void DicomInstanceHasher::Setup(const std::string& patientId,
                                    const std::string& studyUid,
                                    const std::string& seriesUid,
                                    const std::string& instanceUid)
    {
        patientId_ = patientId;
        studyUid_ = studyUid;
        seriesUid_ = seriesUid;
        instanceUid_ = instanceUid;

        if (studyUid_.size() == 0 ||
            seriesUid_.size() == 0 ||
            instanceUid_.size() == 0)
        {
        throw OrthancException(ErrorCode_BadFileFormat, "missing StudyInstanceUID, SeriesInstanceUID or SOPInstanceUID");
        }
    }

    DicomInstanceHasher::DicomInstanceHasher(const DicomMap& instance)
    {
        const DicomValue* patientId = instance.TestAndGetValue(DICOM_TAG_PATIENT_ID);

        Setup(patientId == NULL ? "" : patientId->GetContent(),
            instance.GetValue(DICOM_TAG_STUDY_INSTANCE_UID).GetContent(),
            instance.GetValue(DICOM_TAG_SERIES_INSTANCE_UID).GetContent(),
            instance.GetValue(DICOM_TAG_SOP_INSTANCE_UID).GetContent());
    }

    DicomInstanceHasher::DicomInstanceHasher(const std::string &patientId,
                                             const std::string &studyUid,
                                             const std::string &seriesUid,
                                             const std::string &instanceUid)
    {
        Setup(patientId, studyUid, seriesUid, instanceUid);
    }

    const std::string &DicomInstanceHasher::GetPatientId() const
    {
        return patientId_;
    }

    const std::string &DicomInstanceHasher::GetStudyUid() const
    {
        return studyUid_;
    }

    const std::string &DicomInstanceHasher::GetSeriesUid() const
    {
        return seriesUid_;
    }

    const std::string &DicomInstanceHasher::GetInstanceUid() const
    {
        return instanceUid_;
    }

    const std::string& DicomInstanceHasher::HashPatient() const
    {
        if (patientHash_.size() == 0)
        {
            Toolbox::ComputeSHA1(patientHash_, patientId_);
        }
        return patientHash_;
    }

    const std::string& DicomInstanceHasher::HashStudy() const
    {
        if (studyHash_.size() == 0)
        {
            Toolbox::ComputeSHA1(studyHash_, patientId_ + "|" + studyUid_);
        }
        return studyHash_;
    }

    const std::string& DicomInstanceHasher::HashSeries() const
    {
        if (seriesHash_.size() == 0)
        {
            Toolbox::ComputeSHA1(seriesHash_, patientId_ + "|" + studyUid_ + "|" + seriesUid_);
        }
        return seriesHash_;
    }

    const std::string& DicomInstanceHasher::HashInstance() const
    {
        if (instanceHash_.size() == 0)
        {
            Toolbox::ComputeSHA1(instanceHash_, patientId_ + "|" + studyUid_ + "|" + seriesUid_ + "|" + instanceUid_);
        }
        return instanceHash_;
    }

}