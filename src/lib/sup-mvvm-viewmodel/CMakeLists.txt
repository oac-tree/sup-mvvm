set(library_name sup-mvvm-viewmodel)
set(export_name viewmodel)

add_library(${library_name} SHARED "")
add_library(sup-mvvm::${export_name} ALIAS ${library_name}) # alias for build-tree usage
set_target_properties(${library_name} PROPERTIES EXPORT_NAME ${export_name} SOVERSION ${SUP_MVVM_SOVERSION} VERSION ${SUP_MVVM_BUILDVERSION})

set(export_filename ${SUP_MVVM_AUTOGEN_DIR}/mvvm/viewmodel_export.h)
generate_export_header(${library_name} EXPORT_FILE_NAME ${export_filename}  EXPORT_MACRO_NAME MVVM_VIEWMODEL_EXPORT)

add_subdirectory(mvvm)

target_link_libraries(${library_name} PUBLIC sup-mvvm-model Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::Widgets)
if (SUP_MVVM_BUILD_QCUSTOMPLOT)
  target_link_libraries(${library_name} PRIVATE qcustomplot)
endif()

target_include_directories(${library_name} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<BUILD_INTERFACE:${SUP_MVVM_AUTOGEN_DIR}>)

# -- Installation --
install(TARGETS ${library_name} EXPORT sup-mvvm-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY mvvm/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm FILES_MATCHING PATTERN "*.h")
install(FILES ${export_filename} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm)
