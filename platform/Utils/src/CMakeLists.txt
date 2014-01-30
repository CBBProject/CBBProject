find_package(Boost REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

if (WIN32)
    include_directory(${CMAKE_CURRENT_SOURCE_DIR})
    add_library(dl STATIC dlfcn.c)
endif()

add_library(Plugins SHARED Plugins.C)
target_link_libraries(Plugins ${Boost_LIBRARIES})

install(TARGETS Plugins
    EXPORT Utils
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
)
#INSTALL(EXPORT  Utils   DESTINATION ${INSTALL_DATA_DIR})
