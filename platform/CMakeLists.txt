find_package(dtk REQUIRED)

include(${dtk_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/interface ${CMAKE_BINARY_DIR})
subdirs(Utils Images interface app)
