include(CTest)
include(NewExecutable)

macro(TESTS ConfigName)

    parse_arguments("LIBRARIES" "" "DEFAULT" ${ARGN})

    if (${ConfigName} STREQUAL ${CMAKE_PROJECT_NAME})
        set(OPTION_NAME BUILD_TESTS)
    else()
        string(TOUPPER ${ConfigName} ConfigNameUPPER)
        set(OPTION_NAME BUILD_${ConfigNameUPPER}_TESTS)
    endif()

    option(${OPTION_NAME} "Build ${ConfigName} tests" OFF)

    if (BUILD_TEST OR ${OPTION_NAME})
        foreach(TEST ${DEFAULT})
            add_executable(${ConfigName}-Test-${TEST} ${TEST}.C)
            target_link_libraries(${ConfigName}-Test-${TEST} ${LIBRARIES})
            add_test(${ConfigName}-Test-${TEST} ${ConfigName}-Test-${TEST})
        endforeach()

        option(ENABLE_COVERAGE "Enable coverage" OFF)
        mark_as_advanced(ENABLE_COVERAGE)

        if (ENABLE_COVERAGE)
            if (USE_GCC)
                set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
                set(CMAKE_CXX_FLAGS "-g -O0 ${GCC_WARNING_OPTIONS} ${CMAKE_EXE_LINKER_FLAGS}")
                set(cmake_c_flags "-G -o0 -wALL -w ${cmake_exe_linker_flags}")
            else()
                message(SEND_ERROR "Coverage is only available with gcc.")
            endif()
        endif()
    endif()

endmacro()
