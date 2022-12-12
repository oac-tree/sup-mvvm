set(test testmvvm_model)

file(GLOB_RECURSE source_files "*.cpp")
file(GLOB_RECURSE include_files "*.h")

#file(GLOB source_files "experimental/*.cpp")
#file(GLOB include_files "experimental/*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE test-utils)

gtest_discover_tests(${test} TEST_PREFIX "Model.")

