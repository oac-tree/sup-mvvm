# tests for all classes from sup-mvvm-view (require QApplication)

set(test testsup-mvvm-view)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE sup-mvvm::view sup-mvvm::test sup-mvvm-test-utils)
if (SUP_MVVM_BUILD_QCUSTOMPLOT)
  target_link_libraries(${test} PRIVATE qcustomplot)
endif()

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} TEST_PREFIX "View." DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST)

