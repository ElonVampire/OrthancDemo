#####################################################################
## Versioning information
#####################################################################

# Version of the build, should always be "mainline" except in release branches
set(ORTHANC_VERSION "1.11.3")

# Version of the database schema. History:
#   * Orthanc 0.1.0 -> Orthanc 0.3.0 = no versioning
#   * Orthanc 0.3.1                  = version 2
#   * Orthanc 0.4.0 -> Orthanc 0.7.2 = version 3
#   * Orthanc 0.7.3 -> Orthanc 0.8.4 = version 4
#   * Orthanc 0.8.5 -> Orthanc 0.9.4 = version 5
#   * Orthanc 0.9.5 -> mainline      = version 6
set(ORTHANC_DATABASE_VERSION 6)

# Version of the Orthanc API, can be retrieved from "/system" URI in
# order to check whether new URI endpoints are available even if using
# the mainline version of Orthanc
set(ORTHANC_API_VERSION "19")

#####################################################################
## CMake parameters tunable by the user
#####################################################################




#####################################################################
## Internal CMake parameters to enable the optional subcomponents of
## the Orthanc framework
#####################################################################



set(ORTHANC_SANDBOXED OFF CACHE INTERNAL
  "Whether Orthanc runs inside a sandboxed environment (such as Google NaCl or WebAssembly)")

set(ORTHANC_BUILDING_FRAMEWORK_LIBRARY OFF CACHE INTERNAL
  "Whether we are in the process of building the Orthanc Framework shared library")