set(library_name mvvm_model)

find_package(LibXml2 REQUIRED)

add_library(${library_name} SHARED "")
add_subdirectory(mvvm)
add_library(MVVM::Model ALIAS ${library_name}) # alias for build-tree usage

# -- Generate header for export --

set(export_filename ${MVVM_AUTOGEN_DIR}/mvvm/model_export.h)
generate_export_header(${library_name} EXPORT_FILE_NAME ${export_filename})

# Qt dependency because of uniqueidgenerator.cpp (see comments there)
check_include_file_cxx("experimental/filesystem" HAVE_STD_EXPERIMENTAL_FILESYSTEM)

if (HAVE_STD_EXPERIMENTAL_FILESYSTEM)
  target_link_libraries(${library_name} PRIVATE stduuid PUBLIC Qt5::Widgets LibXml2::LibXml2 -lstdc++fs)
else()
  target_link_libraries(${library_name} PRIVATE stduuid PUBLIC  Qt5::Widgets LibXml2::LibXml2)
endif()

target_include_directories(${library_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<BUILD_INTERFACE:${MVVM_AUTOGEN_DIR}>)

# -- Installation --

install(TARGETS ${library_name} EXPORT mvvm-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(${library_name} PROPERTIES EXPORT_NAME Model SOVERSION ${MVVM_SOVERSION} VERSION ${MVVM_BUILDVERSION})
install(DIRECTORY mvvm/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm FILES_MATCHING PATTERN "*.h")
install(FILES ${export_filename} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm)
