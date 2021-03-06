function(IncludePackage NAME)
    FIND_PACKAGE(${NAME} PATHS ${CMAKE_CURRENT_BINARY_DIR}/../${NAME} NO_DEFAULT_PATH)
    FIND_PACKAGE(${NAME})
    IF (${${NAME}_USE_FILE})
        INCLUDE(${${NAME}_USE_FILE})
    ELSE()
        MESSAGE(ERROR "Package ${NAME} not found !")
    ENDIF()
endfunction()

