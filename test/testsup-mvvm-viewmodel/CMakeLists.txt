# tests for all classes from sup-mvvm-viewmodel

set(test testsup-mvvm-viewmodel)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE sup-mvvm::test sup-mvvm-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST TEST_PREFIX "ViewModel.")

