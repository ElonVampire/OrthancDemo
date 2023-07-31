
#include "../PrecompiledHeaders.h"
#include "DicomTag.h"

#include "../OrthancException.h"

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>

namespace Orthanc
{
    static inline uint16_t GetCharValue(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        else if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        else
            return 0;        
    }

    static inline uint16_t GetTagValue(const char* c)
    {
        return ((GetCharValue(c[0]) << 12) + 
                (GetCharValue(c[1]) << 8) + 
                (GetCharValue(c[2]) << 4) + 
                GetCharValue(c[3]));
    }

    
    DicomTag::DicomTag(uint16_t group, uint16_t element) :
        group_(group),
        element_(element)
    {
    }

    uint16_t DicomTag::GetGroup() const
    {
        return group_;
    }

    uint16_t DicomTag::GetElement() const
    {
        return element_;
    }

    bool DicomTag::IsPrivate() const
    {
        return group_ % 2 == 1;
    }

    bool DicomTag::operator< (const DicomTag& other) const
    {
        if (group_ < other.group_)
            return true;
        
        if (group_ > other.group_)
            return false;

        return element_ < other.element_;
    }

    bool DicomTag::operator<= (const DicomTag& other) const
    {
        if (group_ < other.group_)
            return true;
        
        if (group_ > other.group_)
            return false;

        return element_ <= other.element_;
    }

    bool DicomTag::operator> (const DicomTag& other) const
    {
        return !(*this <= other);
    }

    bool DicomTag::operator>= (const DicomTag& other) const
    {
        return !(*this < other);
    }

    bool DicomTag::operator== (const DicomTag& other) const
    {
        return group_ == other.group_ && element_ == other.element_;
    }

    bool DicomTag::operator!= (const DicomTag& other) const
    {
        return !(*this == other);
    }

    std::ostream& DicomTag::FormatStream(std::ostream& o) const
    {
        using namespace std;
        ios_base::fmtflags state = o.flags();
        o.flags(ios::right | ios::hex);
        o << "(" << setfill('0') << setw(4) << GetGroup()
          << "," << setw(4) << GetElement() << ")";
        o.flags(state);
        return o;
    }

    std::string DicomTag::Format() const
    {
        char b[16];
        sprintf(b, "%04x,%04x", group_, element_);
        return std::string(b);
    }

    bool DicomTag::ParseHexadecimal(DicomTag& tag, const char* value)
    {
        size_t length = strlen(value);

        if (length == 9 &&
            isxdigit(value[0]) && isxdigit(value[1]) &&
            isxdigit(value[2]) && isxdigit(value[3]) &&
            (value[4] == '-' || value[4] == ',') &&
            isxdigit(value[5]) && isxdigit(value[6]) &&
            isxdigit(value[7]) && isxdigit(value[8]))
        {
            
        }
    }

}