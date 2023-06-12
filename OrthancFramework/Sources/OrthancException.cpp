
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
            
        }
    }
}