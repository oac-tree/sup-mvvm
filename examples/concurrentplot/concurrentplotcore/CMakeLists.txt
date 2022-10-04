# library: concurrentplotcore
set(library_name concurrentplotcore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE sup-mvvm::view)

target_sources(${library_name} PRIVATE
    graph_model.cpp
    graph_model.h
    graph_widget.cpp
    graph_widget.h
    graph_widget_toolbar.cpp
    graph_widget_toolbar.h
    job_manager.cpp
    job_manager.h
    mainwindow.cpp
    mainwindow.h
    toy_simulation.cpp
    toy_simulation.h
)

target_include_directories(${library_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

