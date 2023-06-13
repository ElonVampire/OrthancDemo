
#include "PrecompiledHeaders.h"
#include "OrthancException.h"

#include "Logging.h"

namespace Orthanc
{
    OrthancException::OrthancException(const OrthancException& other) :
        errorCode_(other.errorCode_),
        httpStatus_(other.httpStatus_),
        logged_(false)
    {
        if(other.details_.get() != NULL)
        {
            details_.reset(new std::string(*other.details_));
        }
    }

    OrthancException::OrthancException(ErrorCode errorCode) :
        errorCode_(errorCode),
        httpStatus_(ConvertErrorCodeToHttpStatus(errorCode)),
        logged_(false)
    {
    }

    OrthancException::OrthancException(ErrorCode errorCode, const std::string& details, bool log) :
        errorCode_(errorCode),
        httpStatus_(ConvertErrorCodeToHttpStatus(errorCode)),
        logged_(log),
        details_(new std::string(details))
    {
#if ORTHANC_ENABLE_LOGGING == 1
        if(log)
        {
            LOG(ERROR) << EnumerationToString(errorCode_) << ": " << details;
        }
#endif
    }

    OrthancException::OrthancException(ErrorCode errorCode, HttpStatus httpStatus) :
        errorCode_(errorCode),
        httpStatus_(httpStatus),
        logged_(false)
    {
    }

    OrthancException::OrthancException(ErrorCode errorCode, HttpStatus httpStatus, const std::string& details, bool log):
        errorCode_(errorCode),
        httpStatus_(httpStatus),
        logged_(log),
        details_(new std::string(details))
    {
#if ORTHANC_ENABLE_LOGGING == 1
        if(log)
        {
            LOG(ERROR) << EnumerationToString(errorCode_) << ": " << details;
        }
#endif
    }

    ErrorCode OrthancException::GetErrorCode() const
    {
        return errorCode_;
    }

    HttpStatus OrthancException::GetHttpStatus() const
    {
        return httpStatus_;
    }

    const char* OrthancException::What() const
    {
        return EnumerationToString(errorCode_);
    }

    bool OrthancException::HasDetails() const
    {
        return details_.get() != NULL;
    }

    const char* OrthancException::GetDetails() const
    {
        if(details_.get() == NULL)
        {
            return "";
        }
        else
        {
            return details_->c_str();
        }
    }

    bool OrthancException::HasBeenLogged() const
    {
        return logged_;
    }

}