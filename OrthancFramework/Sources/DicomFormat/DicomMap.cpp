
#include "../PrecompiledHeaders.h"
#include "DicomMap.h"

#include <stdio.h>
#include <memory>
#include <boost/algorithm/string/join.hpp>

#include "../Compatibility.h"
#include "../Endianness.h"
#include "../Logging.h"
#include "../OrthancException.h"
#include "../Toolbox.h"
//#include "DicomArray.h"

#if ORTHANC_ENABLE_DCMTK == 1
#include "../DicomParsing/FromDcmtkBridge.h"
#endif

namespace Orthanc
{
    // WARNING: the DEFAULT list of main dicom tags below are the list as they 
    // were in Orthanc 1.10 before we introduced the dynamic main dicom tags.
    // This list has not changed since Orthanc 1.4.2 and had a single change since
    // Orthanc 0.9.5.
    // These lists have a specific signature.  When a resource does not have
    // the metadata "MainDicomTagsSignature", we'll assume that they were stored
    // with an Orthanc prior to 1.11.  It is therefore very important that you never
    // change these lists !

    static const DicomTag DEFAULT_PATIENT_MAIN_DICOM_TAGS[] =
    {
        // { DicomTag(0x0010, 0x1010), "PatientAge" },
        // { DicomTag(0x0010, 0x1040), "PatientAddress" },
        DICOM_TAG_PATIENT_NAME,
        DICOM_TAG_PATIENT_BIRTH_DATE,
        DICOM_TAG_PATIENT_SEX,
        DICOM_TAG_OTHER_PATIENT_IDS,
        DICOM_TAG_PATIENT_ID,
    };

    static const DicomTag DEFAULT_STUDY_MAIN_DICOM_TAGS[] =
    {
        // { DicomTag(0x0010, 0x1020), "PatientSize" },
        // { DicomTag(0x0010, 0x1030), "PatientWeight" },
        DICOM_TAG_STUDY_DATE,
        DICOM_TAG_STUDY_TIME,
        DICOM_TAG_STUDY_ID,
        DICOM_TAG_STUDY_DESCRIPTION,
        DICOM_TAG_ACCESSION_NUMBER,
        DICOM_TAG_STUDY_INSTANCE_UID,
        // New in db v6
        DICOM_TAG_REQUESTED_PROCEDURE_DESCRIPTION,
        DICOM_TAG_INSTITUTION_NAME,
        DICOM_TAG_REQUESTING_PHYSICIAN,
        DICOM_TAG_REFERRING_PHYSICIAN_NAME
    };

    static const DicomTag DEFAULT_SERIES_MAIN_DICOM_TAGS[] =
    {
        // { DicomTag(0x0010, 0x1080), "MilitaryRank" },
        DICOM_TAG_SERIES_DATE,
        DICOM_TAG_SERIES_TIME,
        DICOM_TAG_MODALITY,
        DICOM_TAG_MANUFACTURER,
        DICOM_TAG_STATION_NAME,
        DICOM_TAG_SERIES_DESCRIPTION,
        DICOM_TAG_BODY_PART_EXAMINED,
        DICOM_TAG_SEQUENCE_NAME,
        DICOM_TAG_PROTOCOL_NAME,
        DICOM_TAG_SERIES_NUMBER,
        DICOM_TAG_CARDIAC_NUMBER_OF_IMAGES,
        DICOM_TAG_IMAGES_IN_ACQUISITION,
        DICOM_TAG_NUMBER_OF_TEMPORAL_POSITIONS,
        DICOM_TAG_NUMBER_OF_SLICES,
        DICOM_TAG_NUMBER_OF_TIME_SLICES,
        DICOM_TAG_SERIES_INSTANCE_UID,

            // New in db v6
        DICOM_TAG_IMAGE_ORIENTATION_PATIENT,
        DICOM_TAG_SERIES_TYPE,
        DICOM_TAG_OPERATOR_NAME,
        DICOM_TAG_PERFORMED_PROCEDURE_STEP_DESCRIPTION,
        DICOM_TAG_ACQUISITION_DEVICE_PROCESSING_DESCRIPTION,
        DICOM_TAG_CONTRAST_BOLUS_AGENT
    };

    static const DicomTag DEFAULT_INSTANCE_MAIN_DICOM_TAGS[] =
    {
        DICOM_TAG_INSTANCE_CREATION_DATE,
        DICOM_TAG_INSTANCE_CREATION_TIME,
        DICOM_TAG_ACQUISITION_NUMBER,
        DICOM_TAG_IMAGE_INDEX,
        DICOM_TAG_INSTANCE_NUMBER,
        DICOM_TAG_NUMBER_OF_FRAMES,
        DICOM_TAG_TEMPORAL_POSITION_IDENTIFIER,
        DICOM_TAG_SOP_INSTANCE_UID,

        // New in db v6
        DICOM_TAG_IMAGE_POSITION_PATIENT,
        DICOM_TAG_IMAGE_COMMENTS,

        /**
         * Main DICOM tags that are not part of any release of the
         * database schema yet, and that will be part of future db v7. In
         * the meantime, the user must call "/tools/reconstruct" once to
         * access these tags if the corresponding DICOM files where
         * indexed in the database by an older version of Orthanc.
         **/
        DICOM_TAG_IMAGE_ORIENTATION_PATIENT  // New in Orthanc 1.4.2
    };

    

    class DicomMap::MainDicomTagsConfiguration
    {
    private:
        friend DicomMap;

        std::set<DicomTag>      patientsMainDicomTagsByLevel_;
        std::set<DicomTag>      studiesMainDicomTagsByLevel_;
        std::set<DicomTag>      seriesMainDicomTagsByLevel_;
        std::set<DicomTag>      instancesMainDicomTagsByLevel_;

        std::set<DicomTag>      allMainDicomTags_;

        std::map<ResourceType, std::string> signatures_;
        std::map<ResourceType, std::string> defaultSignatures_;

        MainDicomTagsConfiguration()
        {
            ResetDefaultMainDicomTags();
        }

        void ResetDefaultMainDicomTags()
        {
            patientsMainDicomTagsByLevel_.clear();
            studiesMainDicomTagsByLevel_.clear();
            seriesMainDicomTagsByLevel_.clear();
            instancesMainDicomTagsByLevel_.clear();

            allMainDicomTags_.clear();

            // by default, initialize with the previous static list (up to 1.10.0)
            LoadDefaultMainDicomTags(ResourceType_Patient);
            LoadDefaultMainDicomTags(ResourceType_Study);
            LoadDefaultMainDicomTags(ResourceType_Series);
            LoadDefaultMainDicomTags(ResourceType_Instance);

            defaultSignatures_[ResourceType_Patient] = signatures_[ResourceType_Patient];
            defaultSignatures_[ResourceType_Study] = signatures_[ResourceType_Study];
            defaultSignatures_[ResourceType_Series] = signatures_[ResourceType_Series];
            defaultSignatures_[ResourceType_Instance] = signatures_[ResourceType_Instance];
        }

        std::string ComputeSignature(const std::set<DicomTag>& tags)
        {
            // std::set are sorted by default (which is important for us !)
            std::set<std::string> tagsIds;
            for (std::set<DicomTag>::const_iterator it = tags.begin(); it != tags.end(); ++it)
            {
                tagsIds.insert(it->Format());
            }
            std::string signatureText = boost::algorithm::join(tagsIds, ";");
            return signatureText;
        } 

        void LoadDefaultMainDicomTags(ResourceType level)
        {
            const DicomTag* tags = NULL;
            size_t size;

            switch (level)
            {
            case ResourceType_Patient
                tags = DEFAULT_PATIENT_MAIN_DICOM_TAGS;
                break;
            
            default:
                break;
            }
        }


    };

}