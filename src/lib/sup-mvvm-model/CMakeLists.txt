set(library_name sup-mvvm-model)

find_package(LibXml2 REQUIRED)

add_library(${library_name} SHARED "")
add_subdirectory(mvvm)
add_library(sup-mvvm::model ALIAS ${library_name}) # alias for build-tree usage

# -- Generate header for export --

set(export_filename ${SUP_MVVM_AUTOGEN_DIR}/mvvm/model_export.h)
generate_export_header(${library_name} EXPORT_FILE_NAME ${export_filename} EXPORT_MACRO_NAME MVVM_MODEL_EXPORT)

target_link_libraries(${library_name} PRIVATE stduuid PUBLIC LibXml2::LibXml2)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1))
message(VERBOSE "Linking stdc++fs")
target_link_libraries(${library_name} PRIVATE stdc++fs)
endif ()

target_include_directories(${library_name} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<BUILD_INTERFACE:${SUP_MVVM_AUTOGEN_DIR}>)

# -- Installation --

install(TARGETS ${library_name} EXPORT sup-mvvm-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(${library_name} PROPERTIES EXPORT_NAME model SOVERSION ${SUP_MVVM_SOVERSION} VERSION ${SUP_MVVM_BUILDVERSION})
install(DIRECTORY mvvm/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm FILES_MATCHING PATTERN "*.h")
install(FILES ${export_filename} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm)
