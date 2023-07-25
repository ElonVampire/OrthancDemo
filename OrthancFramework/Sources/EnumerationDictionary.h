
#pragma

#include "OrthancException.h"

#include "Toolbox.h"
#include <boost/lexical_cast.hpp>
#include <string>
#include <map>

namespace Orthanc
{
    template <typename Enumeration>
    class EnumerationDictionary
    {
    private:
        typedef std::map<Enumeration, std::string> EnumerationToString;
        typedef std::map<std::string, Enumeration> StringToEnumeration;

        EnumerationToString enumerationToString_;
        StringToEnumeration stringToEnumeration_;

    public:
        void Clear()
        {
            enumerationToString_.clear();
            stringToEnumeration_.clear();
        }

        bool Contains(Enumeration value) const
        {
            return enumerationToString_.find(value) != enumerationToString_.end();
        }

        void Add(Enumeration value, const std::string& str)
        {
            //check if these values are free.
            if (enumerationToString_.find(value) != enumerationToString_.end() ||
                stringToEnumeration_.find(value) != stringToEnumeration_.end() ||
                Toolbox::IsInteger(str))
            {
                throw OrthancException(ErrorCode_BadRequest);
            }

            //OK, the string is free and is not a number
            enumerationToString_[value] = str;
            stringToEnumeration_[str] = value;
            stringToEnumeration_[boost::lexical_cast<std::string>(static_cast<int>(value))] = value;
        }

        Enumeration Translate(const std::string& str) const
        {
            if (Toolbox::IsInteger(str))
            {
                return static_cast<Enumeration>(boost::lexical_cast<int>(str));
            }

            typename StringToEnumeration::const_iterator
                found = stringToEnumeration_.find(str);
            
            if (found == stringToEnumeration_.end())
            {
                throw OrthancException(ErrorCode_InexistentItem);
            }
            else
            {
                return found->second;
            }
        }

        std::string Translate(Enumeration e) const
        {
            typename EnumerationToString::const_iterator
                found = enumerationToString_.find(e);
            
            if (found == enumerationToString_.end())
            {
                //No name for this item
                return boost::lexical_cast<std::string>(static_cast<int>(e));
            }
            else
            {
                return found->second;
            }
        }
    };
}