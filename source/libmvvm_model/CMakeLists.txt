set(library_name mvvm_model)

add_library(${library_name} SHARED "")
add_subdirectory(mvvm)

# Qt dependency because of uniqueidgenerator.cpp (see comments there)
target_link_libraries(${library_name} Qt5::Widgets)

target_include_directories(${library_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
