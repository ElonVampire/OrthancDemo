

#pragma once

#include "../../OrthancFramework/Sources/FileStorage/IStorageArea.h"
#include "../../OrthancFramework/Sources/Logging.h"
#include "Database/IDatabaseWrapper.h"

namespace Orthanc
{
    void OrthancInitialize(const char* configurationFile = NULL);

    void OrthancFinalize();

    IDatabaseWrapper* CreateDatabaseWrapper();

    IStorageArea* CreateStorageArea();

    void SetGlobalVerbosity(Verbosity verbosity);

    Verbosity GetGlobalVerbosity();

    void SetCategoryVerbosity(Logging::LogCategory category, Verbosity verbosity);

    Verbosity GetCategoryVerbosity(Logging::LogCategory category);

}