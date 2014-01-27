set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

project(Utils C CXX)
cmake_minimum_required(VERSION 2.8)

set(CPACK_RPM_USER_BINARY_SPECFILE packaging/Utils.spec.in)

include(PackageVersion)

PackageVersion(Utils 1 1 svn)

include(CTest)
include(GenerateConfigFile)
include(Packaging)

Dependencies(Utils)

include_directories(include)
sub_directories(include src)

# Help other projects use Utils

GenerateConfigFile(Utils LIBRARIES Plugins)
