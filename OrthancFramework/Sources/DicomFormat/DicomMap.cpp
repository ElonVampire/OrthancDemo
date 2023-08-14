
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
            case ResourceType_Patient:
                tags = DEFAULT_PATIENT_MAIN_DICOM_TAGS;
                size = sizeof(DEFAULT_PATIENT_MAIN_DICOM_TAGS) / sizeof(DicomTag);
                break;
            
            case ResourceType_Study:
                tags = DEFAULT_STUDY_MAIN_DICOM_TAGS;
                size = sizeof(DEFAULT_STUDY_MAIN_DICOM_TAGS) / sizeof(DicomTag);
                break;

            case ResourceType_Series:
                tags = DEFAULT_SERIES_MAIN_DICOM_TAGS;
                size = sizeof(DEFAULT_SERIES_MAIN_DICOM_TAGS) / sizeof(DicomTag);
                break;

            case ResourceType_Instance:
                tags = DEFAULT_INSTANCE_MAIN_DICOM_TAGS;
                size = sizeof(DEFAULT_INSTANCE_MAIN_DICOM_TAGS) / sizeof(DicomTag);
                break;

            default:
                throw OrthancException(ErrorCode_ParameterOutOfRange);
            }
            assert(tags != NULL && size != 0);

            for (size_t i = 0; i < size; i++)
            {
                AddMainDicomTag(tags[i] level);
            }
        }

        std::set<DicomTag>& GetMainDicomTagsByLevel(ResourceType level)
        {
            switch (level)
            {
            case ResourceType_Patient:
                return patientsMainDicomTagsByLevel_;

            case ResourceType_Study:
                return studiesMainDicomTagsByLevel_;

            case ResourceType_Series:
                return seriesMainDicomTagsByLevel_;

            case ResourceType_Instance:
                return instancesMainDicomTagsByLevel_;

            default:
                throw OrthancException(ErrorCode_InternalError);
            }
        }

    public:
        //singleton pattern
        static MainDicomTagsConfiguration& GetInstance()
        {
            static MainDicomTagsConfiguration parameters;
            return parameters;
        }

        void AddMainDicomTag(const DicomTag& tag, ResourceType level)
        {
            const std::set<DicomTag>& existingLevelTags = GetMainDicomTagsByLevel(level);

            if (existingLevelTags.find(tag) != existingLevelTags.end())
            {
                throw OrthancException(ErrorCode_MainDicomTagsMultiplyDefined, tag.Format() + " is already defined");
            }

            GetMainDicomTagsByLevel(level).insert(tag);
            allMainDicomTags_.insert(tag);
            signatures_[level] = ComputeSignature(GetMainDicomTagsByLevel(level));
        }

        const std::set<DicomTag>& GetAllMainDicomTags() const
        {
            return allMainDicomTags_;
        }

        const std::string& GetMainDicomTagsSignature(ResourceType level)
        {
            assert(signatures_.find(level) != signatures_.end());

            return signatures_[level];
        }

        const std::string& GetDefaultMainDicomTagsSignature(ResourceType level)
        {
            assert(defaultSignatures_.find(level) != defaultSignatures_.end())

            return defaultSignatures_[level];
        }
    };

    void DicomMap::SetValueInternal(uint16_t group, uint16_t element, DicomValue* value)
    {
        DicomTag tag(group, element);
        Content::iterator it = content_.find(tag);

        if (it != content_.end())
        {
            delete it->second;
            it->second = value;
        }
        else
        {
            content_.insert(std::make_pair(tag, value));
        }
    }

    void DicomMap::Clear()
    {
        for (Content::iterator it = content_.begin(); it != content_.end(); ++it)
        {
            assert(it->second != NULL);
            delete it->second;
        }
        content_.clear();
    }

    void DicomMap::SetNullValue(uint16_t group, uint16_t element)
    {
        SetValueInternal(group, element, new DicomValue);
    }

    void DicomMap::SetNullValue(const DicomTag& tag)
    {
        SetValueInternal(tag.GetGroup(), tag.GetElement(), new DicomValue);
    }

    void DicomMap::SetValue(uint16_t group, uint16_t element, const DicomValue& value)
    {
        SetValueInternal(group, element, value.Clone());
    }

    void DicomMap::SetValue(const DicomTag& tag, const DicomValue& value)
    {
        SetValueInternal(tag.GetGroup(), tag.GetElement(), value.Clone());
    }

    void DicomMap::SetValue(const DicomTag& tag, const std::string& str, bool isBinary)
    {
        SetValueInternal(tag.GetGroup(), tag.GetElement(), new DicomValue(str, isBinary));
    }

    void DicomMap::SetValue(uint16_t group, uint16_t element, const std::string &str, bool isBinary)
    {
        SetValueInternal(group, element, new DicomValue(str, isBinary));
    }

    void DicomMap::SetValue(const DicomTag& tag, const Json::Value& value)
    {
        SetValueInternal(tag.GetGroup(), tag.GetElement(), new DicomValue(value));
    }

    bool DicomMap::HasTag(uint16_t group, uint16_t element) const
    {
        return HasTag(DicomTag(group, element));
    }

    bool DicomMap::HasTag(const DicomTag& tag) const
    {
        return content_.find(tag) != content_.end();
    }

    const DicomValue& DicomMap::GetValue(uint16_t group, uint16_t element) const
    {
        return GetValue(DicomTag(group, element));
    }

    const DicomValue& DicomMap::GetValue(const DicomTag& tag) const
    {
        const DicomValue* value = TestAndGetValue(tag);

        if (value)
        {
            return *value;
        }
        else
        {
            throw OrthancException(ErrorCode_InexistentTag);
        }
    }

    const DicomValue* DicomMap::TestAndGetValue(uint16_t group, uint16_t element) const
    {
        return TestAndGetValue(DicomTag(group, element));
    }

    const DicomValue* DicomMap::TestAndGetValue(const DicomTag& tag) const
    {
        Content::const_iterator it = content_.find(tag);

        if (it == content_.end())
        {
            return NULL;
        }
        else
        {
            return it->second;
        }
    }

    static void ExtractTagsInternal(DicomMap& result, const DicomMap::Content& source, const std::set<DicomTag>& mainDicomTags)
    {
        result.Clear();

        for (std::set<DicomTag>::const_iterator itmt = mainDicomTags.begin(); itmt != mainDicomTags.end(); ++itmt)
        {
            DicomMap::Content::const_iterator it = source.find(*itmt);
            if (it != source.end())
            {
                result.SetValue(it->first, *it->second  /* value will be cloned */));
            }
        }
    }

    void DicomMap::ExtractTags(DicomMap& result, const std::set<DicomTag>& tags) const
    {
        result.Clear();

        for (std::set<DicomTag>::const_iterator itmt = tags.begin(); itmt != tags.end(); ++itmt)
        {
            DicomMap::Content::const_iterator it = content_.find(*itmt);
            if (it != content_.end())
            {
                result.SetValue(it->first, *it->second);
            }
        }
    }

    void DicomMap::ExtractResourceInformation(DicomMap& result, ResourceType level) const
    {
        const std::set<DicomTag>& mainDicomTags = DicomMap::MainDicomTagsConfiguration::GetInstance().GetMainDicomTagsByLevel(level);
        ExtractTagsInternal(result, content_, mainDicomTags);
    }

    void DicomMap::ExtractPatientInformation(DicomMap& result) const
    {
        ExtractResourceInformation(result, ResourceType_Patient);
    }

    void DicomMap::ExtractStudyInformation(DicomMap& result) const
    {
        ExtractResourceInformation(result, ResourceType_Study);
    }

    void DicomMap::ExtractSeriesInformation(DicomMap& result) const
    {
        ExtractResourceInformation(result, ResourceType_Series);
    }

    void DicomMap::ExtractInstanceInformation(DicomMap& result) const
    {
        ExtractResourceInformation(result, ResourceType_Instance);
    }


    DicomMap::~DicomMap()
    {
        Clear();
    }

    size_t DicomMap::GetSize() const
    {
        return content_.size();
    }

    DicomMap* DicomMap::Clone() const
    {
        std::unique_ptr<DicomMap> result(new DicomMap);

        for (Content::const_iterator it = content_.begin(); it != content_.end(); ++it)
        {
            result->content_.insert(std::make_pair(it->first, it->second));
        }

        return result.release();
    }

    void DicomMap::Assign(const DicomMap& other)
    {
        Clear();

        for (Content::const_iterator it = other.content_.begin(); it != other.content_.end(); ++it)
        {
            content_.insert(std::make_pair(it->first, it->second));
        }
    }

    void DicomMap::Remove(const DicomTag& tag)
    {
        Content::iterator it = content_.find(tag);
        if (it != content_.end())
        {
            delete it->second;
            content_.erase(it);
        }
    }

    void DicomMap::RemoveTags(const std::set<DicomTag>& tags)
    {
        for (std::set<DicomTag>::const_iterator it = tags.begin(); it != tags.end(); ++it)
        {
            Remove(*it);
        }
    }

    static void SetupFindTemplate(DicomMap& result, const std::set<DicomTag>& mainDicomTags)
    {
        result.Clear();

        for (std::set<DicomTag>::const_iterator itmt = mainDicomTags.begin(); itmt != mainDicomTags.end(); ++itmt)
        {
            result.SetValue(*itmt, "", false);
        }
    }

    void DicomMap::SetupFindPatientTemplate(DicomMap& result)
    {
        const std::set<DicomTag>& mainDicomTags = DicomMap::MainDicomTagsConfiguration::GetInstance().GetMainDicomTagsByLevel(ResourceType_Patient);
        SetupFindTemplate(result, mainDicomTags);
    }

    void DicomMap::SetupFindStudyTemplate(DicomMap& result)
    {
        const std::set<DicomTag>& mainDicomTags = DicomMap::MainDicomTagsConfiguration::GetInstance().GetMainDicomTagsByLevel(ResourceType_Study);
        SetupFindTemplate(result, mainDicomTags);

        result.SetValue(DICOM_TAG_ACCESSION_NUMBER, "", false);
        result.SetValue(DICOM_TAG_PATIENT_ID, "", false);

        // These main DICOM tags are only indirectly related to the
        // General Study Module, remove them  
        result.Remove(DICOM_TAG_INSTITUTION_NAME);
        result.Remove(DICOM_TAG_REQUESTING_PHYSICIAN);
        result.Remove(DICOM_TAG_REQUESTED_PROCEDURE_DESCRIPTION);
    }

    void DicomMap::SetupFindSeriesTemplate(DicomMap& result)
    {
        const std::set<DicomTag>& mainDicomTags = DicomMap::MainDicomTagsConfiguration::GetInstance().GetMainDicomTagsByLevel(ResourceType_Series);
        SetupFindTemplate(result, mainDicomTags);
        result.SetValue(DICOM_TAG_ACCESSION_NUMBER, "", false);
        result.SetValue(DICOM_TAG_PATIENT_ID, "", false);
        result.SetValue(DICOM_TAG_STUDY_INSTANCE_UID, "", false);

        //These tags are considered as "main" by Orthanc, but are not in the Series Module.
        result.Remove(DicomTag(0x0008, 0x0070));    //Manufacturer
        result.Remove(DicomTag(0x0008, 0x1010));    //Station name
        result.Remove(DicomTag(0x0018, 0x0024));    //Sequence name
        result.Remove(DICOM_TAG_CARDIAC_NUMBER_OF_IMAGES);
        result.Remove(DICOM_TAG_IMAGES_IN_ACQUISITION);
        result.Remove(DICOM_TAG_NUMBER_OF_SLICES);
        result.Remove(DICOM_TAG_NUMBER_OF_TEMPORAL_POSITIONS);
        result.Remove(DICOM_TAG_NUMBER_OF_TIME_SLICES);
        result.Remove(DICOM_TAG_IMAGE_ORIENTATION_PATIENT);
        result.Remove(DICOM_TAG_SERIES_TYPE);
        result.Remove(DICOM_TAG_ACQUISITION_DEVICE_PROCESSING_DESCRIPTION);
        result.Remove(DICOM_TAG_CONTRAST_BOLUS_AGENT);
    }

    void DicomMap::SetupFindInstanceTemplate(DicomMap& result)
    {
        const std::set<DicomTag>& mainDicomTags = DicomMap::MainDicomTagsConfiguration::GetInstance().GetMainDicomTagsByLevel(ResourceType_Instance);
        SetupFindTemplate(result, mainDicomTags);
        result.SetValue(DICOM_TAG_ACCESSION_NUMBER, "", false);
        result.SetValue(DICOM_TAG_PATIENT_ID, "", false);
        resutl.SetValue(DICOM_TAG_STUDY_INSTANCE_UID, "", false);
        result.SetValue(DICOM_TAG_SERIES_INSTANCE_UID, "", false);
    }

    void DicomMap::CopyTagIfExists(const DicomMap& source, const DicomTag& tag)
    {
        if (source.HasTag(tag))
        {
            SetValue(tag, source.GetValue(tag));
        }
    }

    bool DicomMap::IsMainDicomTag(const DicomTag& tag, ResourceType level)
    {
        const std::set<DicomTag>& mainDicomTags = DicomMap::MainDicomTagsConfiguration::GetInstance().GetMainDicomTagsByLevel(level);
        return mainDicomTags.find(tag) != mainDicomTags.end();
    }

    bool DicomMap::IsMainDicomTag(const DicomTag& tag)
    {
        return (IsMainDicomTag(tag, ResourceType_Patient) ||
                IsMainDicomTag(tag, ResourceType_Study) ||
                IsMainDicomTag(tag, ResourceType_Series) ||
                IsMainDicomTag(tag, ResourceType_Instance));
    }

    static bool IsGenericComputedTag(const DicomTag& tag)
    {
        return tag == DICOM_TAG_RETRIEVE_URL || tag == DICOM_TAG_RETRIEVE_AE_TITLE;
    }

    bool DicomMap::IsComputedTag(const DicomTag& tag)
    {
        return (IsComputedTag(tag, ResourceType_Patient) ||
                IsComputedTag(tag, ResourceType_Study) ||
                IsComputedTag(tag, ResourceType_Series) ||
                IsComputedTag(tag, ResourceType_Instance) ||
                IsGenericComputedTag(tag));
    }

    bool DicomMap::IsComputedTag(const DicomTag& tag, ResourceType level)
    {
        switch (level)
        {
        case ResourceType_Patient:
            return (
                tag == DICOM_TAG_NUMBER_OF_PATIENT_RELATED_STUDIES ||
                tag == DICOM_TAG_NUMBER_OF_PATIENT_RELATED_SERIES ||
                tag == DICOM_TAG_NUMBER_OF_PATIENT_RELATED_INSTANCES
            );
        case ResourceType_Study:
            return (
                tag == DICOM_TAG_MODALITIES_IN_STUDY ||
                tag == DICOM_TAG_SOP_CLASSES_IN_STUDY ||
                tag == DICOM_TAG_NUMBER_OF_STUDY_RELATED_INSTANCES ||
                tag == DICOM_TAG_NUMBER_OF_STUDY_RELATED_SERIES
            );        
      case ResourceType_Series:
            return (
                tag == DICOM_TAG_NUMBER_OF_SERIES_RELATED_INSTANCES
            );
      case ResourceType_Instance:
            return (
                tag == DICOM_TAG_INSTANCE_AVAILABILITY
            );
      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
        }
    }

    bool DicomMap::HasOnlyComputedTags(const std::set<DicomTag>& tags)
    {
        if (tags.size() == 0)
        {
            return false;
        }

        for (std::set<DicomTag>::const_iterator it = tags.begin() ; it != tags.end(); ++it)
        {
            if (!IsComputedTag(*it))
            {
                return false;
            }
        }
        return true;
    }

    bool DicomMap::HasComputedTags(const std::set<DicomTag>& tags)
    {
        for (std::set<DicomTag>::const_iterator it = tags.begin(); it != tags.end(); ++it)
        {
            if (IsComputedTag(*it))
            {
                return true;
            }
        }
        return false;
    }

    bool DicomMap::HasComputedTags(const std::set<DicomTag>& tags, ResourceType level)
    {
        for (std::set<DicomTag>::const_iterator it = tags.begin(); it != tags.end(); ++it)
        {
            if (IsComputedTag(*it, level))
            {
                return true;
            }
        }
        return false;
    }

    const std::set<DicomTag>& DicomMap::GetMainDicomTags(ResourceType level)
    {
        return DicomMap::MainDicomTagsConfiguration::GetInstance().GetMainDicomTagsByLevel(level);
    }

    const std::set<DicomTag>& DicomMap::GetAllMainDicomTags()
    {
        return DicomMap::MainDicomTagsConfiguration::GetInstance().GetAllMainDicomTags();
    }

    void DicomMap::AddMainDicomTag(const DicomTag& tag, ResourceType level)
    {
        DicomMap::MainDicomTagsConfiguration::GetInstance().AddMainDicomTag(tag, level);
    }

    void DicomMap::ResetDefaultMainDicomTags()
    {
        DicomMap::MainDicomTagsConfiguration::GetInstance().ResetDefaultMainDicomTags();
    }

    const std::string& DicomMap::GetMainDicomTagsSignature(ResourceType level)
    {
        return DicomMap::MainDicomTagsConfiguration::GetInstance().GetMainDicomTagsSignature(level);
    }

    const std::string& DicomMap::GetDefaultMainDicomTagsSignature(ResourceType level)
    {
        return DicomMap::MainDicomTagsConfiguration::GetInstance().GetDefaultMainDicomTagsSignature(level);
    }

    void DicomMap::GetTags(std::set<DicomTag>& tags) const
    {
        tags.clear();

        for (Content::const_iterator it = content_.begin(); it != content_.end(); ++it)
        {
            tags.insert(it->first);
        }
    }

    static uint16_t ReadLittleEndianUint16(const char* dicom)
    {
        
    }

}