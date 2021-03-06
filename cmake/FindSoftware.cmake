macro(find type arg)
    set(guard "${arg}")
    if ("${type}" STREQUAL "package")
        set(guard "${arg}_FOUND")
    endif()
    if ("${type}" STREQUAL "package")
        if (NOT ${guard})
            find_package(${arg} ${ARGN})
        endif()
    elseif ("${type}" STREQUAL "library")
        find_library(${arg} ${arg} ${ARGN})
    else()
        message(SEND_ERROR "Unknown type ${type}")
    endif()
endmacro()
