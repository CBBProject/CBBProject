set(ENV{IMAGE_IO_PLUGINS} "${CMAKE_CURRENT_BINARY_DIR}/../plugins/check.plugins")
unset(ENV{IMAGE_IO_PLUGINS_VERBOSE})

function(COMPARE NAME OUTPUT REFERENCE)

    set(COMPARISON_OUTPUT ${NAME}-Comparison.output)
    set(COMPARISON_ERROR ${NAME}-Comparison.error)

    if ("${REFERENCE}" STREQUAL "Null.output")
        file(READ ${OUTPUT} data)
        string(LENGTH "${data}" COMPARE_STATUS)
    else()
        if ("${TEST_COMPARISON}" STREQUAL "file_comparison")
            set(COMPARISON_COMMAND ${CMAKE_COMMAND} -E compare_files)
        else()
            set(COMPARISON_COMMAND "${CMAKE_BINARY_DIR}/../../../bin/Compare")
        endif()

        message("${TEST_COMPARISON} ${OUTPUT} ${REFERENCE}")
        execute_process(COMMAND ${COMPARISON_COMMAND} ${OUTPUT} ${REFERENCE}
                        RESULT_VARIABLE COMPARE_STATUS
                        OUTPUT_FILE ${COMPARISON_OUTPUT}
                        ERROR_FILE ${COMPARISON_ERROR}
        )
    endif()

    message("${COMP_STATUS} | ${COMPARE_STATUS}")
    if ("${COMP_STATUS}" MATCHES "^[0-9]+$" AND
        "${COMPARE_STATUS}" MATCHES "^[0-9]+$")
        math(EXPR COMP_STATUS "${COMP_STATUS} | ${COMPARE_STATUS}")
    else()
        set(COMP_STATUS 1)
    endif()

    if (${COMP_STATUS} STREQUAL "0")
        filE(REMOVE ${COMPARISON_OUTPUT} ${COMPARISON_ERROR})
    endif()

    set(COMP_STATUS ${COMP_STATUS} PARENT_SCOPE)
endfunction()

#   Tokenize input parameters with respect to simple shell rules.

set(TEST_STRING "${TEST_PROGRAM} ${TEST_ARGS}")
set(TEST_ARGUMENTS)
while (TEST_ARGS)
    string(STRIP ${TEST_ARGS} TEST_ARGS)
    string(LENGTH ${TEST_ARGS} l)

    if (NOT l EQUAL 0)
        string(SUBSTRING ${TEST_ARGS} 0 1 ESCAPE)
        if ("${ESCAPE}" STREQUAL "'")
            string(REGEX REPLACE "'([^']*)'.*" "\\1" ARG       ${TEST_ARGS})
            string(REGEX REPLACE "'[^']*'(.*)" "\\1" TEST_ARGS ${TEST_ARGS})
        elseif ("${ESCAPE}" STREQUAL "\"")
            string(REGEX REPLACE "(\"[^\"]*\").*" "\\1" ARG       ${TEST_ARGS})
            string(REGEX REPLACE "\"[^\"]*\"(.*)" "\\1" TEST_ARGS ${TEST_ARGS})
        else()
            string(REGEX REPLACE "([^ \t]+)[ \t]*.*" "\\1" ARG       ${TEST_ARGS})
            string(REGEX REPLACE "[^ \t]+[ \t]*(.*)" "\\1" TEST_ARGS ${TEST_ARGS})
        endif()
        set(TEST_ARGUMENTS ${TEST_ARGUMENTS} ${ARG})
    endif()
endwhile()

if ("${TEST_INPUT}" STREQUAL "")
    set(EXTRA_PARAM)
    message("${TEST_PROGRAM} ${TEST_ARGUMENTS}")
else()
    set(EXTRA_PARAM INPUT_FILE ${TEST_INPUT})
    message("${TEST_PROGRAM} ${TEST_ARGUMENTS} <  ${TEST_INPUT}")
endif()

execute_process(COMMAND ${TEST_WRAPPER} ${TEST_PROGRAM} ${TEST_ARGUMENTS}
                RESULT_VARIABLE COMP_STATUS
                OUTPUT_FILE     ${TEST_OUTPUT}
                ERROR_FILE      ${TEST_ERROR}
                ${EXTRA_PARAM}
)

string(REGEX REPLACE "[ \t]+" ";" TEST_REFERENCE "${TEST_REFERENCE}")
list(GET TEST_REFERENCE 0 REFERENCE)
COMPARE(${TEST_PROGRAM} ${TEST_OUTPUT} ${REFERENCE})
list(REMOVE_AT TEST_REFERENCE 0 ${TEST_REFERENCE})
foreach (reference ${TEST_REFERENCE})
    string(REGEX REPLACE "(images|results)/" "" test_file ${reference})
    COMPARE(${TEST_PROGRAM} ${test_file} ${reference})
endforeach()

if (${COMP_STATUS} STREQUAL "0")
    message(STATUS "Success")
    file(REMOVE ${TEST_OUTPUT})
else()
    message(STATUS "FAILURE")
endif()
