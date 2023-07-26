
#include "PrecompiledHeadersServer.h"

#include "OrthancInitialization.h"

#include "ServerEnumerations.h"

using namespace Orthanc;


static void PrintErrorCode(ErrorCode code, const char* description)
{
    std::cout
        << std::right << std::setw(16)
        << static_cast<int>(code)
        << "   " << description << std::endl;
}

static void PrintErrors(const char* path)
{
    std::cout
        << path << " " << ORTHANC_VERSION << std::endl
        << "Orthanc, lightweight, RESTful DICOM Server for healthcare and medical reserch."
        << std::endl << std::endl
        << "List of error codes that could be returned by Orthanc:"
        << std::endl << std::endl;


    std::cout << std::endl;
}

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

    const char* configurationFile = NULL;
    /**
     * Parse the command-line options.
     **/ 

    for (int i = 0; i < argc; i++)
    {
        std::string argument(argv[i]);

        if (argument.empty())
        {
            // Ignore empty arguments
        }
        else if (argument[0] != '-')
        {
            if (configurationFile != NULL)
            {
                LOG(ERROR) << "More than one configuration path were provided on the command line, aborting";
                return -1
            }
            else
            {
                // Use the first argument that does not start with a "-" as
                // the configuration file

                // TODO WHAT IS THE ENCODING?
                configurationFile = argv[i];
            }
        }
        else if (argument == "--errors")
        {
            PrintErrors(argv[0]);
            return 0;
        }
    }


    return 0;
}