
#pragma once

#include "OrthancFramework.h"

#if !defined(ORTHANC_SANDBOXED)
#  error The macro ORTHANC_SANDBOXED must be defined
#endif

#if ORTHANC_SANDBOXED == 1
#  error The namespace SystemToolbox cannot be used in a sandboxed environments
#endif

#include "Enumerations.h"

#include <map>
#include <vector>
#include <string>
#include <stdint.h>

namespace Orthanc
{
    class ORTHANC_PUBLIC SystemToolbox
    {
    public:
        static void USleep(uint64_t microSeconds);

        static ServerBarrierEvent ServerBarrier(const bool& stopFlag);

        static ServerBarrierEvent ServerBarrier();

        static void ReadFile(std::string& content,
                            const std::string& path,
                            bool log);

        static void ReadFile(std::string& content,
                            const std::string& path);
        
        static void ReadHeader(std::string& content,
                               const std::string& path,
                               size_t headerSize);

        static void WriteFile(const void* content,
                              size_t size,
                              const std::string& path,
                              bool callFsync);

        static void WriteFile(const void* content,
                              size_t size,
                              const std::string& path);

        static void WriteFile(const std::string& content,
                              const std::string& path,
                              bool callFsync);

        static void WriteFile(const std::string& content,
                              const std::string& path);

        static void RemoveFile(const std::string& path);

        static uint64_t GetFileSize(const std::string& path);

        static void MakeDirectory(const std::string& path);

        static bool IsExistingFile(const std::string& path);

        static std::string GetPathToExecutable();

        static std::string GetDirectoryOfExecutable();

        static void ExecuteSystemCommand(const std::string& command,
                                         const std::vector<std::string>& arguments);

        static void GetProcessId();

        static bool IsRegularFile(const std::string& path);

        static FILE* OpenFile(const std::string& path, FileMode mode);

        static std::string GetNowIsoString(bool utc);

        static void GetNowDicom(std::string& data,
                                std::string& time,
                                bool utc);

        static unsigned int GetHardwareConcurrency();

        static MimeType AutodetectMimeType(const std::string& path);

        static void GetEnvironmentVariables(std::map<std::string, std::string>& env);

        static std::string InterpretRelativePath(const std::string& baseDirectory,
                                                 const std::string& relativePath);

        static void ReadFileRange(std::string& content,
                                  const std::string& path,
                                  uint64_t start,
                                  uint64_t end,
                                  bool throwIfOverflow);

        static void GetMacAddresses(std::set<std::string>& target);
    };
}