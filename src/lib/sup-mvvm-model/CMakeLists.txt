set(library_name sup-mvvm-model)
set(export_name model)

coa_add_library(${library_name})
add_library(sup-mvvm::${export_name} ALIAS ${library_name}) # alias for build-tree usage

add_subdirectory(mvvm)

target_link_libraries(${library_name} PRIVATE stduuid sup-utils-tree-data PUBLIC LibXml2::LibXml2)

if (NOT COA_WEB_ASSEMBLY)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1))
  message(VERBOSE "Linking stdc++fs")
  target_link_libraries(${library_name} PRIVATE stdc++fs)
endif ()
endif()

target_include_directories(${library_name} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<BUILD_INTERFACE:${SUP_MVVM_AUTOGEN_DIR}>)

# -- Installation --
install(TARGETS ${library_name} EXPORT sup-mvvm-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY mvvm/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm FILES_MATCHING PATTERN "*.h")
