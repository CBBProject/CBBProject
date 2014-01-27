SET(Utils_HEADERS Cpu.H CpuUtils.H GeneralizedIterators.H IOInit.H
                  IOUtils.H Plugins.H Types.H Verbose.H triplet.H)

include(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Utils/config.h.in Utils/config.h @ONLY)

set(HEADERS)
foreach(file ${Utils_HEADERS})
    set(HEADERS "${HEADERS};${CMAKE_CURRENT_SOURCE_DIR}/Utils/${file}")
endforeach()

install(FILES ${HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/Utils/config.h
        DESTINATION ${INSTALL_INCLUDE_DIR}/Utils COMPONENT dev)
