set(library_name collidingmicecore)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE sup-mvvm::view)

target_sources(${library_name} PRIVATE
  mainwindow.h
  mainwindow.cpp
  mouse.h
  mouse.cpp
  mousemodel.h
  mousemodel.cpp
  mice.qrc
)

target_include_directories(${library_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)


