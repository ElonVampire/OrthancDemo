cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0058 NEW)

project(Orthanc)

#####################################################################
## Generic parameters of the Orthanc framework
#####################################################################
include(${CMAKE_SOURCE_DIR}/../OrthancFramework/Resources/CMake/OrthancFrameworkParameters.cmake)

# Enable all the optional components of the Orthanc framework
set(ENABLE_CRYPTO_OPTIONS ON)
set(ENABLE_DCMTK ON)
set(ENABLE_DCMTK_NETWORKING ON)
set(ENABLE_DCMTK_TRANSCODING ON)
set(ENABLE_GOOGLE_TEST ON)
set(ENABLE_JPEG ON)
set(ENABLE_LOCALE ON)
set(ENABLE_LUA ON)
set(ENABLE_OPENSSL_ENGINES ON)  # OpenSSL engines are necessary for PKCS11
set(ENABLE_PNG ON)
set(ENABLE_PUGIXML ON)
set(ENABLE_SQLITE ON)
set(ENABLE_WEB_CLIENT ON)
set(ENABLE_WEB_SERVER ON)
set(ENABLE_ZLIB ON)

#####################################################################
## CMake parameters tunable at the command line to configure the
## plugins, the companion tools, and the unit tests
#####################################################################

# Parameters of the build
set(STANDALONE_BUILD                ON CACHE BOOL "Standalone build (all the resources are embedded, necessary for releases)")
SET(BUILD_MODALITY_WORKLISTS        ON CACHE BOOL "Whether to build the sample plugin to serve modality worklists")
SET(BUILD_RECOVER_COMPRESSED_FILE   ON CACHE BOOL "Whether to build the companion tool to recover files compressed using Orthanc")
SET(BUILD_SERVE_FOLDERS             ON CACHE BOOL "Whether to build the ServeFolders plugin")
SET(BUILD_CONNECTIVITY_CHECKS       ON CACHE BOOL "Whether to build the ConnectivityChecks plugin")
SET(BUILD_HOUSEKEEPER               ON CACHE BOOL "Whether to build the Housekeeper plugin")
SET(BUILD_DELAYED_DELETION          ON CACHE BOOL "Whether to build the DelayedDeletion plugin")
SET(ENABLE_PLUGINS                  ON CACHE BOOL "Enable plugins")
SET(UNIT_TESTS_WITH_HTTP_CONNEXIONS ON CACHE BOOL "Allow unit tests to make HTTP requests")


#####################################################################
## Configuration of the Orthanc framework
#####################################################################

include(${CMAKE_SOURCE_DIR}/../OrthancFramework/Resources/CMake/VisualStudioPrecompiledHeaders.cmake)
include(${CMAKE_SOURCE_DIR}/../OrthancFramework/Resources/CMake/OrthancFrameworkConfiguration.cmake)

# To export the proper symbols in the sample plugins
include(${CMAKE_SOURCE_DIR}/Plugins/Samples/Common/OrthancPluginsExports.cmake)