find_package(Boost REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

if (WIN32)
    include_directory(${CMAKE_CURRENT_SOURCE_DIR})
    add_library(dl STATIC dlfcn.c)
endif()

set(Images_LIB_SOURCES Image.C RGBPixel.C ImageIO.C)

add_library(Images SHARED ${Images_LIB_SOURCES})
target_link_libraries(Images)

set_target_properties(Images PROPERTIES
                      VERSION
                      ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}
                      SOVERSION 0
                      CLEAN_DIRECT_OUTPUT 1)

add_definitions(-DIMAGE_IO_PLUGINS_DEFAULT_LIST=\"${PLUGIN_DIRECTORY}/default.plugins\")
add_library(ImagesIOPlugins SHARED ImageIOPlugins.C Plugins.C)
target_link_libraries(ImagesIOPlugins Images ${Boost_LIBRARIES})

set_target_properties(ImagesIOPlugins PROPERTIES
                      VERSION
                      ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}
                      SOVERSION 0
                      CLEAN_DIRECT_OUTPUT 1)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

qt4_wrap_cpp(ImageViewer_SOURCES_MOC ../include/Images/ImageViewer.H)
add_library(ImageViewer SHARED ImageViewer.C ${ImageViewer_SOURCES_MOC})
target_link_libraries(ImageViewer ${QT_LIBRARIES} Images ImagesIOPlugins)

set_target_properties(ImageViewer PROPERTIES
                      VERSION
                      ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}
                      SOVERSION 0
                      CLEAN_DIRECT_OUTPUT 1)

install(TARGETS Images ImageViewer ImagesIOPlugins EXPORT Images
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
        COMPONENT runtime)
