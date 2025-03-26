set(library_name customeditorscore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PRIVATE sup-mvvm::view)

target_sources(${library_name} PRIVATE
    main_window.cpp
    main_window.h
)
