set(library_name sup-mvvm-view)

add_library(${library_name} SHARED "")
add_subdirectory(mvvm)
add_library(sup-mvvm::view ALIAS ${library_name}) # alias for build-tree usage

# -- Generate header for export --

set(export_filename ${SUP_MVVM_AUTOGEN_DIR}/mvvm/view_export.h)
generate_export_header(${library_name} EXPORT_FILE_NAME ${export_filename}  EXPORT_MACRO_NAME MVVM_VIEW_EXPORT)

target_link_libraries(${library_name} PUBLIC sup-mvvm-viewmodel Qt${QT_VERSION_MAJOR}::Widgets PRIVATE qcustomplot)
target_include_directories(${library_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<BUILD_INTERFACE:${SUP_MVVM_AUTOGEN_DIR}>)

# -- Installation --

install(TARGETS ${library_name} EXPORT sup-mvvm-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(${library_name} PROPERTIES EXPORT_NAME view SOVERSION ${SUP_MVVM_SOVERSION} VERSION ${SUP_MVVM_BUILDVERSION})
install(DIRECTORY mvvm/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm FILES_MATCHING PATTERN "*.h")
install(FILES ${export_filename} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm)
