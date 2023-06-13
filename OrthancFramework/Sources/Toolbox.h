
#pragma once

#include "Enumerations.h"
#include "OrthancFramework.h"

#include <stdint.h>
#include <vector>
#include <string>
#include <json/value.h>


#if !defined(ORTHANC_ENABLE_BASE64)
#   error The macro ORTHANC_ENABLE_BASE64 must be defined
#endif

#if !defined(ORTHANC_ENABLE_LOCALE)
#   error The macro ORTHANC_ENABLE_LOCALE must be defined
#endif

#if !defined(ORTHANC_ENABLE_MD5)
#   error The macro ORTHANC_ENABLE_MD5 must be defined
#endif

#if !defined(ORTHANC_ENABLE_PUGIXML)
#   error The macro ORTHANC_ENABLE_PUGIXML must be defined
#endif

#if !defined(ORTHANC_ENABLE_SSL)
#   error The macro ORTHANC_ENABLE_SSL must be defined
#endif

/**
 * NOTE: GUID vs. UUID
 * The simple answer is: no difference, they are the same thing. Treat
 * them as a 16 byte (128 bits) value that is used as a unique
 * value. In Microsoft-speak they are called GUIDs, but call them
 * UUIDs when not using Microsoft-speak.
 * http://stackoverflow.com/questions/246930/is-there-any-difference-between-a-guid-and-a-uuid
 **/

#if ORTHANC_ENABLE_PUGIXML == 1
#   include <pugixml.hpp>
#endif

namespace Orthanc
{
    typedef std::vector<std::string> UriComponents;

    class NullType
    {
    };

    class ORTHANC_PUBLIC Toolbox
    {
    public:
        class ORTHANC_PUBLIC LinesIterator
        {
        private:
            const std::string&  content_;
            size_t              lineStart_;
            size_t              lineEnd_;

            void FindEndOfLIne();

        public:
            explicit LinesIterator(const std::string& content);

            bool GetLine(std::string& target) const;

            void Next();
        };

        static void ToUpperCase(std::string& s);        //Inplace version

        static void ToLowerCase(std::string& s);        //Inplace version

        static void ToUpperCase(std::string& result, const std::string& source);

        static void ToLowerCase(std::string& result, const std::string& source);

        static void SplitUriComponents(UriComponents& components, const UriComponents& source, size_t fromLevel);

        static void TruncateUri(UriComponents& target, const UriComponents& source, size_t fromLevel);

        static bool IsChildUri(const UriComponents& baseUri, const UriComponents& testedUri);

        static std::string FlattenUri(const UriComponents& components, size_t fromLevel = 0);

        static std::string JoinUri(const std::string& base, const std::string& uri);

#if ORTHANC_ENABLE_MD5 == 1
        static void ComputeMD5(std::string& result, const std::string& data);

        static void ComputeMD5(std::string& result, const void* data, size_t size);
#endif
        static void ComputeSHA1(std::string& result, const std::string& data);

        static void ComputeSHA1(std::string& result, const void* data, size_t size);

        static bool IsSHA1(const void* str, size_t size);

        static bool IsSHA1(const std::string& s);

#if ORTHANC_ENABLE_BASE64 == 1
        static void DecodeBase64(std::string& result, const std::string& data);

        static void EncodeBase64(std::string& result, const std::string& data);

        static bool DecodeDataUriScheme(std::string& mime, std::string& content, const std::string& source);

        static bool EncodeDataUriScheme(std::string& result, const std::string& mime, const std::string& content);
#endif

#if ORTHANC_ENABLE_LOCALE == 1
        static std::string ConvertToUtf8(const std::string& source, Encoding sourceEncoding, bool hasCodeExtensions);

        static std::string ConvertFromUtf8(const std::string& source, Encoding targetEncoding);
#endif
        static bool IsAsciiString(const void* data, size_t size);

        static bool IsAsciiString(const std::string& s);

        static std::string ConvertToAscii(const std::string& source);

        static std::string StripSpaces(const std::string& source);

        //In-place percent-decoding for URL
        static void UrlDecode(std::string& s);

        static Endianness DetectEndianness();

        static std::string WildcardToRegularExpression(const std::string& s);

        static void TokenizString(std::vector<std::string>& result, const std::string& source, char separator);

        static void JoinStrings(std::string& result, std::set<std::string>& source, const char* spearator);

        static void JoinStrings(std::string& result, std::vector<std::string>& source, const char* separator);

        //return true if all element of 'needles' are found in 'haystack'
        template <typename T> static bool IsSetInSet(const std::set<T>& needles, const std::set<T>& haystack)
        {
            for(typename std::set<T>::const_iterator it = needles.begin(); 
                it != needles.end(); ++it)
            {
                if(haystack.count(*it) == 0)
                {
                    return false;
                }
            }
            return true;
        }

        //return the set of elements from 'needles' that are not in 'haystack'
        template <typename T> static size_t GetMissingsFromSet(std::set<T>& missings, const std::set<T>& needles, const std::set<T>& haystack)
        {
            missings.clear();
            for(typename std::set<T>::const_iterator it = needles.begin();
                it != needles.end(); ++it)
            {
                if(haystack.count(*it) == 0)
                {
                    missings.insert(*it);
                }
            }
            return missings.size();
        }

        template <typename T> static void AppendSets(std::set<T>& target, const std::set<T>& toAppend)
        {
            for(typename std::set<T>::const_iterator it = toAppend.begin();
                it != toAppend.end(); ++it)
            {
                target.insert(*it);
            }
        }
    }

}