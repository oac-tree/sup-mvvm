set(library_name behavior-tree-core)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE sup-mvvm::view)

target_sources(${library_name} PRIVATE
    behavior_tree_items.cpp
    behavior_tree_items.h
    behavior_tree_widget.cpp
    behavior_tree_widget.h
    graphics_scene.cpp
    graphics_scene.h
    graphics_view.cpp
    graphics_view.h
    main_window.cpp
    main_window.h
)
