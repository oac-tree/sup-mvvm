set(library_name customeditorscore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE sup-mvvm::view)

target_sources(${library_name} PRIVATE
    custom_model.cpp
    custom_model.h
    editor_widget.cpp
    editor_widget.h
    main_window.cpp
    main_window.h
)
