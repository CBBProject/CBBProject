set(Images_LIB_SOURCES Image.C RGBPixel.C ImageIO.C)

add_library(Images SHARED ${Images_LIB_SOURCES})
target_link_libraries(Images)

set_target_properties(Images PROPERTIES
                      VERSION
                      ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}
                      SOVERSION 0
                      CLEAN_DIRECT_OUTPUT 1)

add_definitions(-DIMAGE_IO_PLUGINS_DEFAULT_LIST=\"${PLUGIN_DIRECTORY}/default.plugins\")
add_library(ImagesIOPlugins SHARED ImageIOPlugins.C)
target_link_libraries(ImagesIOPlugins Images Plugins)

set_target_properties(ImagesIOPlugins PROPERTIES
                      VERSION
                      ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}
                      SOVERSION 0
                      CLEAN_DIRECT_OUTPUT 1)

set(LIB_DESTINATION ${CMAKE_INSTALL_LIBDIR}/Odyssee++)

install(TARGETS Images ImagesIOPlugins EXPORT Images
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
        COMPONENT runtime)
