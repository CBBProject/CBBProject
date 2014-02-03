project(ZViewer)

set(${PROJECT_NAME}_SOURCES ZViewer.C)

if(NOT MSVC)
add_definitions(-Wno-write-strings)
endif(NOT MSVC)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

add_executable(ZViewer ZViewer.C)
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} Images ImageViewer ImagesIOPlugins)

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
