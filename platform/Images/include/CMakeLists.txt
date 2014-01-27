set(Images_HEADERS Boundary.H Convert.H Defs.H Exceptions.H Image.H ImageIO.H Index.H Iterators.H MinMax.H
	MultiDimCounter.H NullPixel.H PixelIO.H PixelsMinMax.H Polymorphic.H Properties.H Region.H RecFilters.H
    RGBPixel.H Shape.H Utils.H)

set(Utils_HEADERS Cpu.H CpuUtils.H GeneralizedIterators.H IOInit.H IOUtils.H InfoTag.H Plugins.H Types.H triplet.H)
set(Maths_HEADERS Arith.H Vectors.H)

set(NORMAL_HEADERS)

foreach(file ${Images_HEADERS})
    SET(NORMAL_HEADERS "${NORMAL_HEADERS};${CMAKE_CURRENT_SOURCE_DIR}/Images/${file}")
endforeach()

foreach(file ${Utils_HEADERS})
    SET(NORMAL_HEADERS "${NORMAL_HEADERS};${CMAKE_CURRENT_SOURCE_DIR}/Utils/${file}")
endforeach()

foreach(file ${Maths_HEADERS})
    SET(NORMAL_HEADERS "${NORMAL_HEADERS};${CMAKE_CURRENT_SOURCE_DIR}/Maths/${file}")
endforeach()

INSTALL(FILES ${NORMAL_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/Images COMPONENT dev)

set(GLOBAL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Image.H)
INSTALL(FILES ${GLOBAL_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev)
