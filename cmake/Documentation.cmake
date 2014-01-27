MACRO(DOCUMENTATION ConfigName)

    IF (${ConfigName} STREQUAL ${CMAKE_PROJECT_NAME})
        SET(OPTION_NAME BUILD_DOCUMENTATION)
    ELSE()
        STRING(TOUPPER ${ConfigName} ConfigNameUPPER)
        SET(OPTION_NAME BUILD_${ConfigNameUPPER}_DOCUMENTATION)
    ENDIF()

    OPTION(${OPTION_NAME} "Build ${ConfigName} documentation" OFF)

    IF (BUILD_DOCUMENTATION OR ${OPTION_NAME})

        INCLUDE(FindDoxygen)

        # Configure the script and the doxyfile, then add target

        IF(NOT DOT_PATH)
            GET_FILENAME_COMPONENT(DOT_PATH ${DOT} PATH)
        ENDIF()

        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        #   The ALL in the next directive is necessary to cope with a CMake bug !!!
        #   Otherwise the target is never activated.

        ADD_CUSTOM_TARGET(${ConfigName}-documentation ALL)

        ADD_DEPENDENCIES(${ConfigName}-documentation ${ConfigName}-doxygen_outputs)

        ADD_CUSTOM_TARGET(${ConfigName}-doxygen_outputs
                          COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                          DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/ DESTINATION share/doc/Odyssee++/${ConfigName}/reference)

    ENDIF()
ENDMACRO()
