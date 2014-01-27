#-----------------------------------------------
# packaging
#-----------------------------------------------

OPTION(ENABLE_PACKAGING "Enable Packaging" ON)

IF(CMAKE_C_COMPILER MATCHES gcc)
    EXEC_PROGRAM(${CMAKE_CXX_COMPILER}
        ARGS -dumpversion
        OUTPUT_VARIABLE PACKAGE_COMPILER)
    SET(PACKAGE_COMPILER gcc-${PACKAGE_COMPILER})
ELSE()
    SET(PACKAGE_COMPILER ${CMAKE_CXX_COMPILER})
ENDIF()

IF (UNIX AND NOT APPLE) # LINUX
    OPTION(BUILD_RPM "Enable RPM Packaging" OFF)
ENDIF()

# Install README
INSTALL(FILES LICENSE.txt README.txt DESTINATION share/doc/Odyssee++/${PROJECT_NAME})

IF(ENABLE_PACKAGING OR BUILD_RPM)

    #   The next two lines are a kludge to make rpm generation work. If you change this,
    #   please check that rpm generation still works.

    SET(CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION_MAJOR})
    SET(CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION_MINOR})
    SET(CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION_PATCH})

    IF (NOT CPACK_INCLUDED)
        INCLUDE(CPack)
        SET(CPACK_INCLUDED 1)
    ENDIF()

    INCLUDE(InstallRequiredSystemLibraries)

    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} Project")
    SET(CPACK_PACKAGE_VENDOR "INRIA")
    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
    SET(CPACK_PACKAGE_CONTACT "Theodore.Papadopoulo-at-inria.fr")

    IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
        SET(CPACK_RPM_PACKAGE_ARCHITECTURE x86_64)
    ELSE()
        SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
        SET(CPACK_RPM_PACKAGE_ARCHITECTURE i386)
    ENDIF()

    SET(PACKAGE_NAME "${PROJECT_NAME}-${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
    IF(UNIX)
        IF (APPLE)
            SET(PACKAGE_NAME ${PACKAGE_NAME}-MacOSX-Intel)
            IF(BUILD_UNIVERSAL)
                SET(PACKAGE_NAME ${PACKAGE_NAME}-Universal)
            ENDIF()
        ELSE()
            SET(PACKAGE_NAME ${PACKAGE_NAME}-Linux.${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
        ENDIF()
    ELSE()
	if (CMAKE_CL_64)
	    set (CPACK_SYSTEM_NAME win64)
            SET(PACKAGE_NAME ${PACKAGE_NAME}-win64-x86_64)
	else (CMAKE_CL_64)
	    set (CPACK_SYSTEM_NAME win32)
            SET(PACKAGE_NAME ${PACKAGE_NAME}-win32-x86)
	endif (CMAKE_CL_64)
    ENDIF()

    SET(PACKAGE_NAME ${PACKAGE_NAME}-${PACKAGE_COMPILER})

    IF(BUILD_SHARED)
        SET(PACKAGE_NAME ${PACKAGE_NAME}-shared)
    ELSE()
        SET(PACKAGE_NAME ${PACKAGE_NAME}-static)
    ENDIF()

    SET(CPACK_PACKAGE_FILE_NAME ${PACKAGE_NAME})

    IF (WIN32)
        # There is a bug in NSIS that does not handle full unix paths properly. Make
        # sure there is at least one set of four (4) backlasshes.
        SET(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
        #SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\openmeeg.gforge.inria.fr")
        #SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\openmeeg.gforge.inria.fr")
        SET(CPACK_NSIS_CONTACT "Theodore.Papadopoulo-at-inria.fr")
        SET(CPACK_NSIS_MODIFY_PATH ON)
        #SET(CPACK_NSIS_MENU_LINKS
        #    "doc/LICENSE.txt" "README"
        #    "http://openmeeg.gforge.inria.fr" "${PROJECT_NAME} homepage"
        #)
    ENDIF()

    SET(CPACK_SOURCE_STRIP_FILES "")

    IF(UNIX AND NOT APPLE)
        SET(CPACK_GENERATOR "TGZ")
    ENDIF()

    IF(APPLE)
        SET(CPACK_GENERATOR "PackageMaker;TGZ")
    ENDIF()

    IF(UNIX AND BUILD_RPM) # linux
        SET(CPACK_GENERATOR "${CPACK_GENERATOR};RPM")
        IF (CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION LESS 10)
            INCLUDE(UseRPMTools)
            IF (RPMTools_FOUND)
                RPMTools_ADD_RPM_TARGETS(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/packaging/${PROJECT_NAME}.spec.in")
            ENDIF()
        ELSE()
            SET(CPACK_RPM_PACKAGE_LICENSE "CeCILL-B")
            SET(CPACK_RPM_PACKAGE_DESCRIPTION  "${PROJECT_NAME} is a package from Odyssee++.")
            SET(CPACK_RPM_PACKAGE_GROUP "System Environment/Libraries")
        ENDIF()
    ENDIF()

ENDIF()

IF (ENABLE_PACKAGING AND WIN32)
    INCLUDE(UseWin32dlls)
ENDIF()
