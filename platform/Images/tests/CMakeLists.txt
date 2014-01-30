set(CMAKE_MODULE_PATH_OLD ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(ImagesTests)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH_OLD})

execute_process(COMMAND cmake -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_BINARY_DIR}/images)
execute_process(COMMAND cmake -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/results ${CMAKE_CURRENT_BINARY_DIR}/results)

set(ALL_TESTS 
    PixelAccess PixelAccess3D BaseImageAccess Iterator3D DomainIterator PixelIterator PixelConstIterator
    Copy Order IOpointer IOuchar2D RawPgmIOuchar2D Convert HalfSize ScaleValues Type Compare Stats
    ConvertPgmToInrimage Inrimage5 FormatConverter Swap ReadWrite Convert3D MultiDimCounter SwapBytes
    Shift LowBits SimpleImage3D ReadColor)

foreach(TEST ${ALL_TESTS})
    IMAGE_UNIT_TEST(${TEST} SOURCES ${TEST}.C LIBRARIES Images ImagesIOPlugins dl)
endforeach()
