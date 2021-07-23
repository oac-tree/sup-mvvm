set(test testmvvm_model)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE mvvm_model testmachinery)

gtest_discover_tests(${test} TEST_PREFIX "Model.")

