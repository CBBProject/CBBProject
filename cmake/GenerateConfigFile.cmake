#   Offer the user the choice of overriding the installation directories

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

if (${LIB64} STREQUAL "TRUE")
    set(LIBSUFFIX 64)
else()
    set(LIBSUFFIX "")
endif()

set(INSTALL_LIB_DIR     lib${LIBSUFFIX}/Odyssee++ CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR     bin                       CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include/Odyssee++         CACHE PATH "Installation directory for header files")
set(INSTALL_DATA_DIR    share/                    CACHE PATH "Installation directory for data files")

mark_as_advanced(INSTALL_LIB_DIR INSTALL_BIN_DIR INSTALL_INCLUDE_DIR INSTALL_DATA_DIR)
 
#   Make relative paths absolute (needed later on)

foreach(p LIB BIN INCLUDE DATA)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

macro(sub_directories)
    foreach (dir ${ARGN})
        add_subdirectory(${dir})
    endforeach()
endmacro()

macro(PARSE_ARGUMENTS_GENERATE LIST_VARS DEFAULT_VAR)
    foreach(var ${LIST_VARS})
        unset(${var})
    endforeach ()

    set(CURRENT_VAR ${DEFAULT_VAR})
    foreach (arg ${ARGN})
        set(skip_this_arg FALSE)
        foreach(var ${LIST_VARS})
            if (${arg} STREQUAL ${var})
                set(CURRENT_VAR ${var})
                set(skip_this_arg TRUE)
                BREAK()
            endif()
        endforeach ()
        if (NOT skip_this_arg)
            set(${CURRENT_VAR} ${${CURRENT_VAR}} ${arg})
        endif()
    endforeach()
endmacro()

macro(Dependencies ConfigName)
    PARSE_ARGUMENTS_GENERATE("OPTIONAL" "DEFAULT" ${ARGN})
    set(DepFileName ${CMAKE_CURRENT_BINARY_DIR}/${ConfigName}Dependencies.cmake)
    file(WRITE ${DepFileName} "")
    set(DEPLIBS)
    set(DEPINCS)
    if (NOT "${DEFAULT}" STREQUAL "")
        set(ExternalDependencies)
        foreach(arg ${DEFAULT})
            if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../${arg})
                set(ExternalDependencies ${ExternalDependencies} ${arg})
            else()
                include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../${arg}/includes ${CMAKE_CURRENT_BINARY_DIR}/../${arg}/includes)
            endif()
        endforeach()
        if (${ExternalDependencies})
            configure_file(${PROJECT_SOURCE_DIR}/cmake/FindSoftware.cmake ${DepFileName} COPYONLY)
            file(APPEND ${DepFileName} "\n")
            foreach(arg ${ExternalDependencies})
                set(HINTS ${PROJECT_BINARY_DIR}/../${arg} ${PROJECT_BINARY_DIR}/../Libs/${arg} ${Odyssee++_BINARY_DIR}/Libs/${arg})
                file(APPEND ${DepFileName} "find(package ${arg} HINTS ${HINTS} NO_CMAKE_PACKAGE_REGISTRY QUIET)\n")
                file(APPEND ${DepFileName} "find(package ${arg} PATHS ${INSTALL_DATA_DIR}/ NO_CMAKE_PACKAGE_REGISTRY QUIET)\n")
                file(APPEND ${DepFileName} "find(package ${arg} REQUIRED)\n\n")
                set(DEPLIBS ${DEPLIBS};\${${arg}_LIBRARIES})
                set(DEPINCS ${DEPINCS};\${${arg}_INCLUDE_DIRS})
            endforeach()
            include(${DepFileName})
        endif()
    endif()
endmacro()

function(GenerateConfigFile ConfigName)

    PARSE_ARGUMENTS_GENERATE("LIBRARIES;INCLUDES;INCLUDE_DIRS;PLUGINS_INCLUDE_DIRS" "DEFAULT" ${ARGN})

    #   Creating files for companion projects

    #   Export the package for use from the build-tree
    #   (this registers the build-tree with a global CMake-registry)

    export(PACKAGE ${ConfigName})

    set(INCLUDE_DIRS_ORIG         "${INCLUDE_DIRS}")
    set(PLUGINS_INCLUDE_DIRS_ORIG "${PLUGINS_INCLUDE_DIRS}")

    set(LIBRARIES "${LIBRARIES}")

    #   Create a XXXConfig.cmake file for the use from the build tree

    set(LIB_DIRS             "${CMAKE_CURRENT_BINARY_DIR}/src")
    set(INCLUDE_DIRS         "${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIRS}" "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include")
    set(PLUGINS_DIR          "${CMAKE_CURRENT_BINARY_DIR}/${PLUGINS_INCLUDE_DIRS}")
    set(PLUGINS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/${PLUGINS_INCLUDE_DIRS}")
    set(CONFIG_DIR           "${CMAKE_CURRENT_BINARY_DIR}")

    configure_file(${CMAKE_SOURCE_DIR}/cmake/XXXConfig.cmake.in
                   "${PROJECT_BINARY_DIR}/${ConfigName}Config.cmake" @ONLY)
    configure_file(${CMAKE_SOURCE_DIR}/cmake/XXXConfigVersion.cmake.in
                   "${PROJECT_BINARY_DIR}/${ConfigName}ConfigVersion.cmake" @ONLY)
 
    #   Install the export set for use with the install-tree

    # install(EXPORT ${ConfigName} DESTINATION "${INSTALL_DATA_DIR}/${ConfigName}/CMake" COMPONENT dev)
 
    #   Create a XXXConfig.cmake file for the use from the install tree and install it.

    set(LIB_DIRS             "${INSTALL_LIB_DIR}")
    set(INCLUDE_DIRS         "${INSTALL_INCLUDE_DIR};${INSTALL_INCLUDE_DIR}/${INCLUDE_DIRS_ORIG}")
    set(PLUGINS_INCLUDE_DIRS "${INSTALL_INCLUDE_DIR}/${ConfigName}/${PLUGINS_INCLUDE_DIRS_ORIG}")
    set(CONFIG_DIR           "${INSTALL_DATA_DIR}/${ConfigName}-Odyssee++/CMake")

    configure_file(${CMAKE_SOURCE_DIR}/cmake/XXXConfig.cmake.in
                   "${CMAKE_CURRENT_BINARY_DIR}/InstallFiles/${ConfigName}Config.cmake" @ONLY)
    configure_file(${CMAKE_SOURCE_DIR}/cmake/XXXConfigVersion.cmake.in
                   "${CMAKE_CURRENT_BINARY_DIR}/InstallFiles/${ConfigName}ConfigVersion.cmake" @ONLY)
    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/InstallFiles/${ConfigName}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/InstallFiles/${ConfigName}ConfigVersion.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${ConfigName}Dependencies.cmake"
            DESTINATION "${CONFIG_DIR}" COMPONENT dev)
endfunction()
