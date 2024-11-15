set(library_name saveloadprojectcore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE sup-mvvm::view)

target_sources(${library_name} PRIVATE
  container_editor_widget.cpp
  container_editor_widget.h
  main_window.cpp
  main_window.h
  main_window_actions.cpp
  main_window_actions.h
  model_editor_widget.cpp
  model_editor_widget.h
  project_pane_widget.cpp
  project_pane_widget.h
  recent_project_widget.cpp
  recent_project_widget.h
  sample_model.cpp
  sample_model.h
)
