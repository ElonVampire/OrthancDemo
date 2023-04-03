cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0058 NEW)

project(Orthanc)

include(${CMAKE_SOURCE_DIR}/../OrthancFramework/Resources/CMake/OrthancFrameworkParameters.cmake)



#####################################################################
## Configuration of the Orthanc framework
#####################################################################

include(${CMAKE_SOURCE_DIR}/../OrthancFramework/Resources/CMake/VisualStudioPrecompiledHeaders.cmake)
include(${CMAKE_SOURCE_DIR}/../OrthancFramework/Resources/CMake/OrthancFrameworkConfiguration.cmake)

