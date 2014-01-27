find_package(Boost REQUIRED COMPONENTS iostreams)
include_directories(${Boost_INCLUDE_DIRS})

function(add_plugin PLUGIN)
    add_library(${PLUGIN} SHARED ${PLUGIN}.C)
    if (${PLUGIN} STREQUAL "Inrimage5")
        target_link_libraries(${PLUGIN} ${Boost_LIBRARIES})
    endif()
    set_target_properties(${PLUGIN} PROPERTIES
                          VERSION 1.0.0
                          SOVERSION 1
                          CLEAN_DIRECT_OUTPUT 1)

    install(TARGETS ${PLUGIN}
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION ${PLUGIN_DIRECTORY}
            LIBRARY DESTINATION ${PLUGIN_DIRECTORY})
endfunction()

function(Plugins)
    set(DEFAULT_PLUGINLIST ${CMAKE_CURRENT_BINARY_DIR}/default.plugins)
    set(CHECK_PLUGINLIST ${CMAKE_CURRENT_BINARY_DIR}/check.plugins)
    file(REMOVE ${DEFAULT_PLUGINLIST} ${CHECK_PLUGINLIST})
    foreach (PLUGIN ${ARGN})
        add_plugin(${PLUGIN})
        file(APPEND ${DEFAULT_PLUGINLIST} "${PLUGIN_DIRECTORY}/lib${PLUGIN}.so\n")
        file(APPEND ${CHECK_PLUGINLIST} "${CMAKE_CURRENT_BINARY_DIR}/lib${PLUGIN}.so\n")
    endforeach()
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/default.plugins DESTINATION ${PLUGIN_DIRECTORY})
endfunction()

set(HEADERS Inrimage5.H Inrimage5Tools.H Inrimage.H Pnm.H RawPgm.H RawPpm.H Vmr.H Vtk.H)

install(FILES ${HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/Images/plugins COMPONENT dev)

Plugins(Inrimage5 Inrimage Pnm RawPgm RawPpm Vtk)

install(TARGETS ${PLUGINS} EXPORT Images
        LIBRARY DESTINATION ${PLUGIN_DIRECTORY}
        ARCHIVE DESTINATION ${PLUGIN_DIRECTORY}
        COMPONENT runtime)
