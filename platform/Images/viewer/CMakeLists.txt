project(ZViewer)

set(${PROJECT_NAME}_HEADERS_MOC ImageViewer.H)
set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS_MOC})

set(${PROJECT_NAME}_SOURCES ImageViewer.C ZViewer.C)

if(NOT MSVC)
add_definitions(-Wno-write-strings)
endif(NOT MSVC)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

qt4_wrap_cpp(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_MOC}
  ${${PROJECT_NAME}_SOURCES_WRAP})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} Images ImagesIOPlugins)

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
