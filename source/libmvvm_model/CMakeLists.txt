set(library_name mvvm_model)

add_library(${library_name} SHARED "")
add_subdirectory(mvvm)
target_link_libraries(${library_name})

target_include_directories(${library_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
