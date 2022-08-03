# library: dragandmovecore
set(library_name dragandmovecore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE MVVM::View)

target_sources(${library_name} PRIVATE
  container_editor_widget.cpp
  container_editor_widget.h
  dragviewmodel.cpp
  dragviewmodel.h
  main_window.cpp
  main_window.h
  model_editor_widget.cpp
  model_editor_widget.h
  sampleitems.cpp
  sampleitems.h
  sample_model.cpp
  sample_model.h
)

