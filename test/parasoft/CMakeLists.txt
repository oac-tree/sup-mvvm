if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

message(STATUS "Configuring Parasoft integration")

if(NOT PARASOFT_DIR)
  if(NOT CODAC_FOUND)
    message(FATAL_ERROR "Parasoft install location is unknown, set PARASOFT_DIR to specify one")
  endif()

  # Default to CODAC parasoft installation location in /opt
  set(PARASOFT_DIR /opt/parasoft CACHE PATH "Parasoft install location")
endif()

# Append parasoft cmake integration location to module path
list(APPEND CMAKE_MODULE_PATH "${PARASOFT_DIR}/cpptest/integration/cmake")

# Enable parasoft project generation
set(CPPTEST_PROJECT ON)

# Include Parasoft integration module
include(cpptest-project)

# Add parasoft project for sup-mvvm
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(sup-mvvm-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
  src-sup-mvvm-model=${CMAKE_SOURCE_DIR}/src/lib/sup-mvvm-model
  src-sup-mvvm-viewmodel=${CMAKE_SOURCE_DIR}/src/lib/sup-mvvm-viewmodel
  src-sup-mvvm-view=${CMAKE_SOURCE_DIR}/src/lib/sup-mvvm-view
  src-sup-mvvm-test=${CMAKE_SOURCE_DIR}/src/lib/sup-mvvm-test
  src-testsup-mvvm-model=${CMAKE_SOURCE_DIR}/test/testsup-mvvm-model
  src-testsup-mvvm-viewmodel=${CMAKE_SOURCE_DIR}/test/testsup-mvvm-viewmodel
  src-testsup-mvvm-viewmodel-app=${CMAKE_SOURCE_DIR}/test/testsup-mvvm-viewmodel-app
  src-testsup-mvvm-view=${CMAKE_SOURCE_DIR}/test/testsup-mvvm-view
  TARGETS sup-mvvm-model sup-mvvm-viewmodel sup-mvvm-view sup-mvvm-test testsup-mvvm-model testsup-mvvm-viewmodel testsup-mvvm-viewmodel-app testsup-mvvm-view
)

get_target_property(SUP_MVVM_VIEW_INCLUDE_DIRECTORIES sup-mvvm-view INCLUDE_DIRECTORIES)
get_target_property(SUP_MVVM_TEST_INCLUDE_DIRECTORIES sup-mvvm-test INCLUDE_DIRECTORIES)
target_include_directories(sup-mvvm-cpptest PUBLIC ${SUP_MVVM_VIEW_INCLUDE_DIRECTORIES}  ${SUP_MVVM_TEST_INCLUDE_DIRECTORIES})

target_link_libraries(sup-mvvm-cpptest
  PUBLIC
  LibXml2::LibXml2
)
