
#pragma once

#include "Compatibility.h"
#include "Enumerations.h"
#include "OrthancFramework.h"

namespace Orthanc
{
    class ORTHANC_PUBLIC OrthancException
    {
    private:
        OrthancException();
        OrthancException& operator= (const OrthancException&);

        ErrorCode errorCode_;
        HttpStatus httpStatus_;
        bool logged_;

        std::unique_ptr<std::string> details_;

    public:
        OrthancException(const OrthancException& other);

        explicit OrthancException(ErrorCode errorCode);

        OrthancException(ErrorCode errorCode, 
                        const std::string& details, bool log = true);

        OrthancException(ErrorCode errorCode, HttpStatus httpStatus);

        OrthancException(ErrorCode errorCode, HttpStatus httpStatus,
                        const std::string& details, bool log = true);

        ErrorCode GetErrorCode() const;

        HttpStatus GetHttpStatus() const;

        const char* What() const;

        bool HasDetails() const;

        const char* GetDetails() const;

        bool HasBeenLogged() const;
    };
}