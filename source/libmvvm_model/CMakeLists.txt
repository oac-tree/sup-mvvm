set(library_name mvvm_model)

find_package(LibXml2 REQUIRED)

add_library(${library_name} SHARED "")
add_subdirectory(mvvm)

# -- Generate header for export --

set(export_filename ${MVVM_AUTOGEN_DIR}/mvvm/model_export.h)
generate_export_header(${library_name} EXPORT_FILE_NAME ${export_filename})

# Qt dependency because of uniqueidgenerator.cpp (see comments there)
check_include_file_cxx("experimental/filesystem" HAVE_STD_EXPERIMENTAL_FILESYSTEM)

if (HAVE_STD_EXPERIMENTAL_FILESYSTEM)
  target_link_libraries(${library_name} Qt5::Widgets LibXml2::LibXml2 -lstdc++fs)
else()
  target_link_libraries(${library_name} Qt5::Widgets LibXml2::LibXml2)
endif()

target_include_directories(${library_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<BUILD_INTERFACE:${MVVM_AUTOGEN_DIR}>)
