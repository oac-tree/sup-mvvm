set(library_name plotgraphscore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE MVVM::View)

target_sources(${library_name} PRIVATE
    graph_model.cpp
    graph_model.h
    graph_widget.cpp
    graph_widget.h
    main_window.cpp
    main_window.h
)
