set(library_name sup-mvvm-test)

find_package(GTest 1.10 QUIET CONFIG)
if (NOT GTest_FOUND)  
  message(WARNING "GTest with version >= \"1.10\" was not found, fetching from internet" )
  fetch_googletest()
endif()
find_package_message(
  GTest_DETAILS
  "Found GTest: ${GTest_DIR} (version \"${GTest_VERSION}\")"
  "[${GTest_FOUND}][${GTest_DIR}][v${GTest_VERSION}]")

add_library(${library_name} STATIC "")
add_subdirectory(mvvm)
add_library(sup-mvvm::test ALIAS ${library_name}) # alias for build-tree usage

target_link_libraries(${library_name} PUBLIC pthread GTest::gtest GTest::gmock sup-mvvm::view Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<BUILD_INTERFACE:${SUP_MVVM_AUTOGEN_DIR}>)

install(TARGETS ${library_name} EXPORT sup-mvvm-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(${library_name} PROPERTIES EXPORT_NAME test SOVERSION ${SUP_MVVM_SOVERSION} VERSION ${SUP_MVVM_BUILDVERSION})
install(DIRECTORY mvvm/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm FILES_MATCHING PATTERN "*.h")
install(FILES ${export_filename} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm)
