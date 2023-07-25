
#include "PrecompiledHeadersServer.h"

#include "OrthancInitialization.h"

#include "ServerEnumerations.h"

using namespace Orthanc;


static bool SetCategoryVerbosity(const Verbosity verbosity, const std::string& category)
{
    Logging::LogCategory c;
    if (LookupCategory(c, category))
    {
        SetCategoryVerbosity(c, verbosity);
        return true;
    }
    else
    {
        return false;
    }
}

int main(int argc, char* argv[])
{
    Logging::Initialize();
    SetGlobalVerbosity(Verbosity_Default);

    /**
     * Parse the command-line options.
     **/ 

    for (int i = 0; i < argc; i++)
    {
        std::string argument(argv[i]);

        if (argument.empty())
        {
            //Ignore empty arguments
        }
        else if (argument[0] != '-')
        {
            
        }
    }


    return 0;
}