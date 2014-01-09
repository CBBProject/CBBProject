macro(PackageVersion Package Major Minor Patch)

    if (NOT CPACK_RPM_USER_BINARY_SPECFILE)
        set(CPACK_RPM_USER_BINARY_SPECFILE packaging/${Package}.spec.in)
    endif()

    set(PACKAGE_VERSION_MAJOR "${Major}")
    set(PACKAGE_VERSION_MINOR "${Minor}")
    set(PACKAGE_VERSION_PATCH "${Patch}")
    set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
endmacro()
