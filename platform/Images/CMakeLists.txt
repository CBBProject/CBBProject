project(Images C CXX)
cmake_minimum_required(VERSION 2.8)

include(PackageVersion)

PackageVersion(Images 1 1 git)

include(CTest)
include(GenerateConfigFile)
include(Packaging)

Dependencies(Images Utils)

include_directories(include plugins ${CMAKE_CURRENT_BINARY_DIR}/include)

set(PLUGIN_DIRECTORY ${INSTALL_LIB_DIR}/Images/plugins)
sub_directories(include src plugins tests doc)

# Help other projects use Images

GenerateConfigFile(Images LIBRARIES Images ImagesIOPlugins PLUGINS_INCLUDE_DIRS plugins)
