set(library_name mvvm_model)

find_package(LibXml2 REQUIRED)

add_library(${library_name} SHARED "")
add_subdirectory(mvvm)

# -- Generate header for export --

set(export_filename ${MVVM_AUTOGEN_DIR}/mvvm/model_export.h)
generate_export_header(${library_name} EXPORT_FILE_NAME ${export_filename})

# Qt dependency because of uniqueidgenerator.cpp (see comments there)
target_link_libraries(${library_name} Qt5::Widgets LibXml2::LibXml2)

target_include_directories(${library_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<BUILD_INTERFACE:${MVVM_AUTOGEN_DIR}>)
