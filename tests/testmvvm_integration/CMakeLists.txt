# Test executable for all classes requiring QApplication

set(test testmvvm_integration)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE test-utils qcustomplot)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} TEST_PREFIX "Integration."  DISCOVERY_TIMEOUT 10)

