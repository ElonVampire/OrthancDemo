
#pragma once

#include "../OrthancFramework.h"

#if !defined(ORTHANC_SANBOXED)
#  error The macro ORTHANC_SANBOXED must be defined
#endif

#if ORTHANC_SANBOXED == 1
#  error The class FilesystemStorage cannot be used in a sandboxed environments
#endif

#include "IStorageArea.h"
#include "../Compatibility.h"

#include <stdint.h>
#include <boost/filesystem.hpp>
#include <set>

namespace Orthanc
{
    class ORTHANC_PUBLIC FilesystemStorage : public IStorageArea
    {
        //TODO REMOVE THIS
        friend class FilesystemHttpSender;
        friend class FileStorageAccessor;
    
    private:
        // The root of the filesystem
        boost::filesystem::path  root_;
        // If true, the file is fsync()ed after being written
        bool                     fsyncOnWrite_;

        // Gets the path of the file corresponding to the given UUID
        boost::filesystem::path  GetPath(const std::string& uuid) const;

        // Initializes the filesystem
        void Setup(const std::string& root);

#if ORTHANC_BUILDING_FRAMEWORK_LIBRARY == 1
    // Alias for binary compatibility with Orthanc Framework 1.7.2 => don't use it anymore
    explicit FilesystemStorage(std::string root);
#endif

#if ORTHANC_BUILDING_FRAMEWORK_LIBRARY == 1
    // Binary compatibility with Orthanc Framework <= 1.8.2
    void Read(std::string& content,
              const std::string& uuid,
              FileContentType type);
#endif 

    public:
        explicit FilesystemStorage(const std::string& root);

        FilesystemStorage(const std::string& root, bool fsyncOnWrite);

        virtual void Create(const std::string& uuid,
                            const void* content,
                            size_t size,
                            FileContentType type) ORTHANC_OVERRIDE;

        virtual IMemoryBuffer* Read(const std::string& uuid,
                                    FileContentType type) ORTHANC_OVERRIDE;

        virutal IMemoryBuffer* ReadRange(const std::string& uuid,
                                         FileContentType type,
                                         uint64_t start,
                                         uint64_t end) ORTHANC_OVERRIDE;

        virtual bool HasReadRange() const ORTHANC_OVERRIDE;

        virtual void Remove(const std::string& uuid,
                            FileContentType type) ORTHANC_OVERRIDE;

        void ListAllFiles(std::set<std::string>& result) const;

        uintmax_t GetSize(const std::string& uuid) const;

        void Clear();

        uintmax_t GetCapacity() const;

        uintmax_t GetAvailableSpace() const;
    };
}