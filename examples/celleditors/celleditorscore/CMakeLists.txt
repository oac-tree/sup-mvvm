set(library_name celleditorscore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE MVVM::View)

target_sources(${library_name} PRIVATE
    main_window.cpp
    main_window.h
    model_editor_widget.cpp
    model_editor_widget.h
    sample_model.cpp
    sample_model.h
)
