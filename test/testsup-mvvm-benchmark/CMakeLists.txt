set(test testsup-mvvm-benchmark)

file(GLOB_RECURSE source_files "*.cpp")
file(GLOB_RECURSE include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE test-utils benchmark::benchmark)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

#gtest_discover_tests(${test} TEST_PREFIX "Model.")

