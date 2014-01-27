INCLUDE(NewExecutable)

FUNCTION(CREATE_TESTS NAME TARGET)
    FILE(STRINGS ${NAME}.C tests REGEX "//[ \t]*Example:[ \t]*[.]/${NAME}" )
    IF (WIN32)
        SET(executable ${EXECUTABLE_OUTPUT_PATH}/${TARGET}${CMAKE_EXECUTABLE_SUFFIX})
    ELSE()
        SET(executable ${CMAKE_CURRENT_BINARY_DIR}/${TARGET})
    ENDIF()

    LIST(LENGTH tests len)
    IF (${len} STREQUAL 0)
        SET(tests "// Example: ./${NAME}")
        SET(len 1)
    ENDIF()

    SET(index 0)
    FOREACH(test ${tests})
        STRING(REGEX REPLACE "//[ \t]*Example:[ \t][.]/${NAME}[ \t]*" "" parameters ${test})
        IF (${parameters} MATCHES "###")
            STRING(REGEX REPLACE "###" "##" parameters ${parameters})
            SET(comparison image_comparison)
        ELSE()
            SET(comparison file_comparison)
        ENDIF()
        IF (${parameters} MATCHES "##")
            STRING(REGEX REPLACE ".*##[ \t]*" "" result ${parameters})
            STRING(REGEX REPLACE "[ \t]*##.*" "" parameters ${parameters})
        ELSE()
            SET(result)
        ENDIF()

        IF (NOT ${len} EQUAL 1)
            SET(TEST_NAME "${NAME}.${index}")
            MATH(EXPR index "${index}+1")
        ENDIF()

        IF (${parameters} MATCHES "[^2]>")
            STRING(REGEX REPLACE ".*[^2]>[ \t]*" "" outfile ${parameters})
            STRING(REGEX REPLACE "(.*)[ \t]*[^2]>.*" "\\1" parameters ${parameters})
        ELSE()
            SET(outfile ${TEST_NAME}.output)
        ENDIF()

        IF (${parameters} MATCHES "2>")
            STRING(REGEX REPLACE ".*2>[ \t]*" "" errorfile ${parameters})
            STRING(REGEX REPLACE "(.*)[ \t]*2>.*" "\\1" parameters ${parameters})
        ELSE()
            SET(errorfile ${TEST_NAME}.output)
        ENDIF()

        IF (${parameters} MATCHES "<")
            STRING(REGEX REPLACE ".*<[ \t]*" "" infile ${parameters})
            STRING(REGEX REPLACE "[ \t]*(.*)[ \t]*<.*" "\\1" parameters ${parameters})
        ELSE()
            SET(infile)
        ENDIF()

        IF ("${result}" STREQUAL "")
            SET(result results/${outfile})
        ENDIF()

        FOREACH (var parameters infile outfile errorfile result)
            IF (${${var}} MATCHES "%(!)?arch")
                STRING(REGEX REPLACE "%arch" "le" ${var} ${${var}})
                STRING(REGEX REPLACE "%!arch" "be" ${var} ${${var}})
            ENDIF()
        ENDFOREACH()

        SET(TESTNAME Images-${TEST_NAME})
        ADD_TEST(${TESTNAME}
                 ${CMAKE_COMMAND} -D TEST_PROGRAM=${executable}
                              -D TEST_ARGS=${parameters}
                              -D TEST_INPUT=${infile}
                              -D TEST_OUTPUT=${outfile}
                              -D TEST_ERROR=${errorfile}
                              -D TEST_REFERENCE=${result}
                              -D TEST_COMPARISON=${comparison}
                              -P ${CMAKE_CURRENT_SOURCE_DIR}/runTest.cmake ) # To compare output to reference file
        #ADD_TEST(${TESTNAME} ${EXECUTABLE} ${PROG_ARGS}) # To perform memcheck tests
        #SET_TESTS_PROPERTIES(${TESTNAME} PROPERTIES DEPENDS ${EXECUTABLE})
        SET_TESTS_PROPERTIES(${TESTNAME} PROPERTIES PASS_REGULAR_EXPRESSION "Success")
    ENDFOREACH()
ENDFUNCTION()

FUNCTION(IMAGE_UNIT_TEST TEST_NAME)
    PARSE_ARGUMENTS("SOURCES;LIBRARIES;PARAMETERS" "" "DEFAULT" ${ARGN})
    SET(TARGET ${TEST_NAME})
    IF (${TEST_NAME} STREQUAL "Inrimage5")
        SET(TARGET "TestInrimage5")
    ENDIF()
    NEW_EXECUTABLE(${TARGET} SOURCES ${SOURCES} LIBRARIES ${LIBRARIES})
    CREATE_TESTS(${TEST_NAME} ${TARGET})
ENDFUNCTION()
