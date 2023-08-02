
#include "PrecompiledHeadersServer.h"

#include "OrthancInitialization.h"

#include "ServerEnumerations.h"

using namespace Orthanc;

static void PrintHelp(const char* path)
{
    std::cout 
        << "Usage: " << path << " [OPTION]... [CONFIGURATION]" << std::endl
        << "Orthanc, lightweight, RESTful DICOM server for healthcare and medical research." << std::endl
        << std::endl
        << "The \"CONFIGURATION\" argument can be a single file or a directory. In the " << std::endl
        << "case of a directory, all the JSON files it contains will be merged. " << std::endl
        << "If no configuration path is given on the command line, a set of default " << std::endl
        << "parameters is used. Please refer to the Orthanc Book for the full " << std::endl
        << "instructions about how to use Orthanc <http://book.orthanc-server.com/>." << std::endl
        << std::endl
        << "Pay attention to the fact that the order of the options is important." << std::endl
        << "Options are read left to right. In particular, options such as \"--verbose\" can " << std::endl
        << "reset the value of other log-related options that were read before." << std::endl
        << std::endl
        << "The recommended set of options to debug DICOM communications is " << std::endl
        << "\"--verbose --trace-dicom --logfile=dicom.log\"" << std::endl
        << std::endl
        << "Command-line options:" << std::endl
        << "  --help\t\tdisplay this help and exit" << std::endl
        << "  --logdir=[dir]\tdirectory where to store the log files" << std::endl
        << "\t\t\t(by default, the log is dumped to stderr)" << std::endl
        << "  --logfile=[file]\tfile where to store the log of Orthanc" << std::endl
        << "\t\t\t(by default, the log is dumped to stderr)" << std::endl
        << "  --config=[file]\tcreate a sample configuration file and exit" << std::endl
        << "\t\t\t(if \"file\" is \"-\", dumps to stdout)" << std::endl
        << "  --errors\t\tprint the supported error codes and exit" << std::endl
        << "  --verbose\t\tbe verbose in logs" << std::endl
        << "  --trace\t\thighest verbosity in logs (for debug)" << std::endl
        << "  --upgrade\t\tallow Orthanc to upgrade the version of the" << std::endl
        << "\t\t\tdatabase (beware that the database will become" << std::endl
        << "\t\t\tincompatible with former versions of Orthanc)" << std::endl
        << "  --no-jobs\t\tdon't restart the jobs that were stored during" << std::endl
        << "\t\t\tthe last execution of Orthanc" << std::endl
        << "  --openapi=[file]\twrite the OpenAPI documentation and exit" << std::endl
        << "\t\t\t(if \"file\" is \"-\", dumps to stdout)" << std::endl
        << "  --cheatsheet=[file]\twrite the cheat sheet of REST API as CSV" << std::endl
        << "\t\t\tand exit (if \"file\" is \"-\", dumps to stdout)" << std::endl
        << "  --version\t\toutput version information and exit" << std::endl
        << std::endl
        << "Fine-tuning of log categories:" << std::endl;
    
    for (size_t i = 0; i < Logging::GetCategoriesCount(); i++)
    {
        const std::string name = Logging::GetCategoryName(i);
        std::cout << "  --version-" << name
                  << "\tbe verbose in logs of category \"" << name << "\"" << std::endl;
        std::cout << "  --trace-" << name
                  << "\tuse highest verbosity for logs of category \"" << name << "\"" << std::endl;
    }

    std::cout
        << std::endl
        << "Exit status:" << std::endl
        << "  0\tif success," << std::endl
#if defined(_WIN32)
        << "  != 0\tif error (use the --errors option to get the list of possible errors)." << std::endl
#else
        << "  -1\tif error (have a look at the logs)." << std::endl
#endif
        << std::endl;
}

static void PrintVersion(const char* path)
{
    std::cout
        << path << " " << ORTHANC_VERSION << std::endl
        << "Copyright (C) 2012-2016 Sebastien Jodogne, Medical Physics Department, University Hospital of Liege (Belgium)" << std::endl
        << "Copyright (C) 2017-2023 Osimis S.A. (Belgium)" << std::endl
        << "Copyright (C) 2021-2023 Sebastien Jodogne, ICTEAM UCLouvain (Belgium)" << std::endl
        << "Licensing GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>." << std::endl
        << "This is free software: you are free to change and redistribute it." << std::endl
        << "There is NO WARRANTY, to the extent permitted by law." << std::endl
        << std::endl
        << "Written by Sebastien Jodogne <s.jodogne@orthanc-labs.com>" << std::endl;
}

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

    // The content of the following brackets is automatically generated
    // by the "Resources/CodeGeneration/GenerateErrorCodes.py" script
    {
        PrintErrorCode(ErrorCode_InternalError, "Internal error");
        PrintErrorCode(ErrorCode_Success, "Success");
        PrintErrorCode(ErrorCode_Plugin, "Error encountered within the plugin engine");
        PrintErrorCode(ErrorCode_NotImplemented, "Not implemented yet");
        PrintErrorCode(ErrorCode_ParameterOutOfRange, "Parameter out of range");
        PrintErrorCode(ErrorCode_NotEnoughMemory, "The server hosting Orthanc is running out of memory");
        PrintErrorCode(ErrorCode_BadParameterType, "Bad type for a parameter");
        PrintErrorCode(ErrorCode_BadSequenceOfCalls, "Bad sequence of calls");
        PrintErrorCode(ErrorCode_InexistentItem, "Accessing an inexistent item");
        PrintErrorCode(ErrorCode_BadRequest, "Bad request");
        PrintErrorCode(ErrorCode_NetworkProtocol, "Error in the network protocol");
        PrintErrorCode(ErrorCode_SystemCommand, "Error while calling a system command");
        PrintErrorCode(ErrorCode_Database, "Error with the database engine");
        PrintErrorCode(ErrorCode_UriSyntax, "Badly formatted URI");
        PrintErrorCode(ErrorCode_InexistentFile, "Inexistent file");
        PrintErrorCode(ErrorCode_CannotWriteFile, "Cannot write to file");
        PrintErrorCode(ErrorCode_BadFileFormat, "Bad file format");
        PrintErrorCode(ErrorCode_Timeout, "Timeout");
        PrintErrorCode(ErrorCode_UnknownResource, "Unknown resource");
        PrintErrorCode(ErrorCode_IncompatibleDatabaseVersion, "Incompatible version of the database");
        PrintErrorCode(ErrorCode_FullStorage, "The file storage is full");
        PrintErrorCode(ErrorCode_CorruptedFile, "Corrupted file (e.g. inconsistent MD5 hash)");
        PrintErrorCode(ErrorCode_InexistentTag, "Inexistent tag");
        PrintErrorCode(ErrorCode_ReadOnly, "Cannot modify a read-only data structure");
        PrintErrorCode(ErrorCode_IncompatibleImageFormat, "Incompatible format of the images");
        PrintErrorCode(ErrorCode_IncompatibleImageSize, "Incompatible size of the images");
        PrintErrorCode(ErrorCode_SharedLibrary, "Error while using a shared library (plugin)");
        PrintErrorCode(ErrorCode_UnknownPluginService, "Plugin invoking an unknown service");
        PrintErrorCode(ErrorCode_UnknownDicomTag, "Unknown DICOM tag");
        PrintErrorCode(ErrorCode_BadJson, "Cannot parse a JSON document");
        PrintErrorCode(ErrorCode_Unauthorized, "Bad credentials were provided to an HTTP request");
        PrintErrorCode(ErrorCode_BadFont, "Badly formatted font file");
        PrintErrorCode(ErrorCode_DatabasePlugin, "The plugin implementing a custom database back-end does not fulfill the proper interface");
        PrintErrorCode(ErrorCode_StorageAreaPlugin, "Error in the plugin implementing a custom storage area");
        PrintErrorCode(ErrorCode_EmptyRequest, "The request is empty");
        PrintErrorCode(ErrorCode_NotAcceptable, "Cannot send a response which is acceptable according to the Accept HTTP header");
        PrintErrorCode(ErrorCode_NullPointer, "Cannot handle a NULL pointer");
        PrintErrorCode(ErrorCode_DatabaseUnavailable, "The database is currently not available (probably a transient situation)");
        PrintErrorCode(ErrorCode_CanceledJob, "This job was canceled");
        PrintErrorCode(ErrorCode_BadGeometry, "Geometry error encountered in Stone");
        PrintErrorCode(ErrorCode_SslInitialization, "Cannot initialize SSL encryption, check out your certificates");
        PrintErrorCode(ErrorCode_DiscontinuedAbi, "Calling a function that has been removed from the Orthanc Framework");
        PrintErrorCode(ErrorCode_BadRange, "Incorrect range request");
        PrintErrorCode(ErrorCode_DatabaseCannotSerialize, "Database could not serialize access due to concurrent update, the transaction should be retried");
        PrintErrorCode(ErrorCode_Revision, "A bad revision number was provided, which might indicate conflict between multiple writers");
        PrintErrorCode(ErrorCode_MainDicomTagsMultiplyDefined, "A main DICOM Tag has been defined multiple times for the same resource level");
        PrintErrorCode(ErrorCode_SQLiteNotOpened, "SQLite: The database is not opened");
        PrintErrorCode(ErrorCode_SQLiteAlreadyOpened, "SQLite: Connection is already open");
        PrintErrorCode(ErrorCode_SQLiteCannotOpen, "SQLite: Unable to open the database");
        PrintErrorCode(ErrorCode_SQLiteStatementAlreadyUsed, "SQLite: This cached statement is already being referred to");
        PrintErrorCode(ErrorCode_SQLiteExecute, "SQLite: Cannot execute a command");
        PrintErrorCode(ErrorCode_SQLiteRollbackWithoutTransaction, "SQLite: Rolling back a nonexistent transaction (have you called Begin()?)");
        PrintErrorCode(ErrorCode_SQLiteCommitWithoutTransaction, "SQLite: Committing a nonexistent transaction");
        PrintErrorCode(ErrorCode_SQLiteRegisterFunction, "SQLite: Unable to register a function");
        PrintErrorCode(ErrorCode_SQLiteFlush, "SQLite: Unable to flush the database");
        PrintErrorCode(ErrorCode_SQLiteCannotRun, "SQLite: Cannot run a cached statement");
        PrintErrorCode(ErrorCode_SQLiteCannotStep, "SQLite: Cannot step over a cached statement");
        PrintErrorCode(ErrorCode_SQLiteBindOutOfRange, "SQLite: Bing a value while out of range (serious error)");
        PrintErrorCode(ErrorCode_SQLitePrepareStatement, "SQLite: Cannot prepare a cached statement");
        PrintErrorCode(ErrorCode_SQLiteTransactionAlreadyStarted, "SQLite: Beginning the same transaction twice");
        PrintErrorCode(ErrorCode_SQLiteTransactionCommit, "SQLite: Failure when committing the transaction");
        PrintErrorCode(ErrorCode_SQLiteTransactionBegin, "SQLite: Cannot start a transaction");
        PrintErrorCode(ErrorCode_DirectoryOverFile, "The directory to be created is already occupied by a regular file");
        PrintErrorCode(ErrorCode_FileStorageCannotWrite, "Unable to create a subdirectory or a file in the file storage");
        PrintErrorCode(ErrorCode_DirectoryExpected, "The specified path does not point to a directory");
        PrintErrorCode(ErrorCode_HttpPortInUse, "The TCP port of the HTTP server is privileged or already in use");
        PrintErrorCode(ErrorCode_DicomPortInUse, "The TCP port of the DICOM server is privileged or already in use");
        PrintErrorCode(ErrorCode_BadHttpStatusInRest, "This HTTP status is not allowed in a REST API");
        PrintErrorCode(ErrorCode_RegularFileExpected, "The specified path does not point to a regular file");
        PrintErrorCode(ErrorCode_PathToExecutable, "Unable to get the path to the executable");
        PrintErrorCode(ErrorCode_MakeDirectory, "Cannot create a directory");
        PrintErrorCode(ErrorCode_BadApplicationEntityTitle, "An application entity title (AET) cannot be empty or be longer than 16 characters");
        PrintErrorCode(ErrorCode_NoCFindHandler, "No request handler factory for DICOM C-FIND SCP");
        PrintErrorCode(ErrorCode_NoCMoveHandler, "No request handler factory for DICOM C-MOVE SCP");
        PrintErrorCode(ErrorCode_NoCStoreHandler, "No request handler factory for DICOM C-STORE SCP");
        PrintErrorCode(ErrorCode_NoApplicationEntityFilter, "No application entity filter");
        PrintErrorCode(ErrorCode_NoSopClassOrInstance, "DicomUserConnection: Unable to find the SOP class and instance");
        PrintErrorCode(ErrorCode_NoPresentationContext, "DicomUserConnection: No acceptable presentation context for modality");
        PrintErrorCode(ErrorCode_DicomFindUnavailable, "DicomUserConnection: The C-FIND command is not supported by the remote SCP");
        PrintErrorCode(ErrorCode_DicomMoveUnavailable, "DicomUserConnection: The C-MOVE command is not supported by the remote SCP");
        PrintErrorCode(ErrorCode_CannotStoreInstance, "Cannot store an instance");
        PrintErrorCode(ErrorCode_CreateDicomNotString, "Only string values are supported when creating DICOM instances");
        PrintErrorCode(ErrorCode_CreateDicomOverrideTag, "Trying to override a value inherited from a parent module");
        PrintErrorCode(ErrorCode_CreateDicomUseContent, "Use \"Content\" to inject an image into a new DICOM instance");
        PrintErrorCode(ErrorCode_CreateDicomNoPayload, "No payload is present for one instance in the series");
        PrintErrorCode(ErrorCode_CreateDicomUseDataUriScheme, "The payload of the DICOM instance must be specified according to Data URI scheme");
        PrintErrorCode(ErrorCode_CreateDicomBadParent, "Trying to attach a new DICOM instance to an inexistent resource");
        PrintErrorCode(ErrorCode_CreateDicomParentIsInstance, "Trying to attach a new DICOM instance to an instance (must be a series, study or patient)");
        PrintErrorCode(ErrorCode_CreateDicomParentEncoding, "Unable to get the encoding of the parent resource");
        PrintErrorCode(ErrorCode_UnknownModality, "Unknown modality");
        PrintErrorCode(ErrorCode_BadJobOrdering, "Bad ordering of filters in a job");
        PrintErrorCode(ErrorCode_JsonToLuaTable, "Cannot convert the given JSON object to a Lua table");
        PrintErrorCode(ErrorCode_CannotCreateLua, "Cannot create the Lua context");
        PrintErrorCode(ErrorCode_CannotExecuteLua, "Cannot execute a Lua command");
        PrintErrorCode(ErrorCode_LuaAlreadyExecuted, "Arguments cannot be pushed after the Lua function is executed");
        PrintErrorCode(ErrorCode_LuaBadOutput, "The Lua function does not give the expected number of outputs");
        PrintErrorCode(ErrorCode_NotLuaPredicate, "The Lua function is not a predicate (only true/false outputs allowed)");
        PrintErrorCode(ErrorCode_LuaReturnsNoString, "The Lua function does not return a string");
        PrintErrorCode(ErrorCode_StorageAreaAlreadyRegistered, "Another plugin has already registered a custom storage area");
        PrintErrorCode(ErrorCode_DatabaseBackendAlreadyRegistered, "Another plugin has already registered a custom database back-end");
        PrintErrorCode(ErrorCode_DatabaseNotInitialized, "Plugin trying to call the database during its initialization");
        PrintErrorCode(ErrorCode_SslDisabled, "Orthanc has been built without SSL support");
        PrintErrorCode(ErrorCode_CannotOrderSlices, "Unable to order the slices of the series");
        PrintErrorCode(ErrorCode_NoWorklistHandler, "No request handler factory for DICOM C-Find Modality SCP");
        PrintErrorCode(ErrorCode_AlreadyExistingTag, "Cannot override the value of a tag that already exists");
        PrintErrorCode(ErrorCode_NoStorageCommitmentHandler, "No request handler factory for DICOM N-ACTION SCP (storage commitment)");
        PrintErrorCode(ErrorCode_NoCGetHandler, "No request handler factory for DICOM C-GET SCP");
        PrintErrorCode(ErrorCode_UnsupportedMediaType, "Unsupported media type");
    }

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
        else if (argument == "--help")
        {
            PrintHelp(argv[0]);
            return 0;
        }
        else if (argument == "--version")
        {
            PrintVersion(argv[0]);
            return 0;
        }

        
    }


    return 0;
}