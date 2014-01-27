project(cbb-platform)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS MainWindow.h)
set(${PROJECT_NAME}_HEADERS_MOC MainWindow.h MainWindow_p.h)
set(${PROJECT_NAME}_SOURCES MainWindow.cpp main.cpp)

find_package(Qt4 REQUIRED)
find_package(dtk REQUIRED)

include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${dtk_INCLUDE_DIRS})

## #################################################################
## Build rules
## #################################################################

qt4_wrap_cpp(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_MOC}
  ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME}
  ${QT_LIBRARIES}
  dtkLog
  dtkCore
  dtkGui
  dtkComposer
  cbbCore
  cbbComposer)

## #################################################################
## Installation
## #################################################################

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION bin
        RUNTIME DESTINATION bin)
