set(library_name mvvm_view)

add_library(${library_name} SHARED "")
add_subdirectory(mvvm)

# -- Generate header for export --

set(export_filename ${MVVM_AUTOGEN_DIR}/mvvm/view_export.h)
generate_export_header(${library_name} EXPORT_FILE_NAME ${export_filename})

target_link_libraries(${library_name} PUBLIC mvvm_viewmodel Qt5::Widgets)
target_include_directories(${library_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<BUILD_INTERFACE:${MVVM_AUTOGEN_DIR}>)
