enable_testing()

include(GoogleTest)

find_package(benchmark QUIET)
if(benchmark_FOUND)
  find_package_message(
    benchmark_details
    "Found benchmark: ${benchmark_DIR} (version \"${benchmark_VERSION}\")"
    "[${benchmark_DIR}][v${benchmark_VERSION}]")
endif()

add_subdirectory(libtest-utils)
add_subdirectory(testsup-mvvm-viewmodel-app)
add_subdirectory(testsup-mvvm-model)
add_subdirectory(testsup-mvvm-viewmodel)
add_subdirectory(testsup-mvvm-view)
if (benchmark_FOUND)
  add_subdirectory(testsup-mvvm-benchmark)
endif()
add_subdirectory(parasoft)

# Add custom target `make testall` which will run test executables one after another.
# This can be used instead of usual `make test` or `ctest` while running tests via an ssh
# connection. In certain cases `make testall` will be faster than `ctest -j4` because of fewer
# instantiations of QApplication.

add_custom_target(testall
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsup-mvvm-view
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsup-mvvm-viewmodel-app
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsup-mvvm-viewmodel
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsup-mvvm-model
)

file(WRITE ${TEST_OUTPUT_DIRECTORY}/test.sh
"#!/bin/bash
export QT_QPA_PLATFORM=offscreen
fail=0
" ${TEST_OUTPUT_DIRECTORY}/testsup-mvvm-view " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testsup-mvvm-viewmodel-app " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testsup-mvvm-viewmodel " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testsup-mvvm-model " || fail=1
exit $fail
")

configure_file(${TEST_OUTPUT_DIRECTORY}/test.sh ${TEST_OUTPUT_DIRECTORY}/test.sh 
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
