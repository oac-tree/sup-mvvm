set(test testsup-mvvm-model)

file(GLOB_RECURSE source_files "*.cpp")
file(GLOB_RECURSE include_files "*.h")

#file(GLOB source_files "experimental/*.cpp")
#file(GLOB include_files "experimental/*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} TEST_PREFIX "Model.")

