set(test testmvvm_view)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE test-utils qcustomplot)

gtest_discover_tests(${test} TEST_PREFIX "View." DISCOVERY_TIMEOUT 10)

