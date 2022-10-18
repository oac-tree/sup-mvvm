set(library_name flatviewcore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE sup-mvvm::view)

target_sources(${library_name} PRIVATE
    flat_widget.cpp
    flat_widget.h
    main_window.cpp
    main_window.h
    model_editor_widget.cpp
    model_editor_widget.h
    sample_model.cpp
    sample_model.h
)
