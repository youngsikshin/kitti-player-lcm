cmake_minimum_required(VERSION 2.8)

file(GLOB qt_path /opt/Qt*)

if(EXISTS ${qt_path})
  set(CMAKE_PREFIX_PATH ${qt_path}/5.6/gcc_64/lib/cmake)
  message("-- CMAKE_PREFIX_PATH is " ${qt_path}/5.6/gcc_64/lib/cmake)
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

FIND_PACKAGE(Qt5 QUIET COMPONENTS Core Xml OpenGL Gui Widgets)

set (QtKitti_CPP window.cpp myglwidget.cpp)
set (QtKitti_HEADER window.h myglwidget.h)
set (QtKitti_UI  window.ui)
set (QtKitti_MAIN main.cpp)

if(Qt5_FOUND)
  QT5_WRAP_UI(UI_HEADERS ${QtKitti_UI})
  QT5_WRAP_CPP(UI_SOURCES ${QtKitti_HEADER})

  include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS}
)
  set(QtKitti_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES}
)
endif()

libhandler_opengl ()

add_library(QtKitti_library 
  ${QtKitti_HEADER} ${QtKitti_CPP}
  ${UI_HEADERS} ${UI_SOURCES}
)

add_executable(kitti-player ${QtKitti_MAIN})

target_link_libraries(QtKitti_library ${QGLVIEWER_LIBRARY} ${QtKitti_LIBRARIES} ${OPENGL_LIBRARY})
target_link_libraries(kitti-player QtKitti_library)