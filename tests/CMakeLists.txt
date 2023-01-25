enable_testing()

find_package(GTest 1.10 QUIET CONFIG)
if (GTest_FOUND)
  message(STATUS "Found GTest: suitable version \"${GTest_VERSION}\", with minimum required \"1.10\"" )
else()
  message(STATUS "GTest with version >= \"1.10\" was not found, fetching from internet" )
  fetch_googletest()
endif()

include(GoogleTest)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Test REQUIRED)
find_package(Threads)

add_subdirectory(libtest-utils)
add_subdirectory(testmvvm_integration)
add_subdirectory(testmvvm_model)
add_subdirectory(testmvvm_viewmodel)
add_subdirectory(testmvvm_view)
if (benchmark_FOUND)
  add_subdirectory(testmvvm-benchmark)
endif()

# Add custom target `make testall` which will run test executables one after another.
# This can be used instead of usual `make test` or `ctest` while running tests via an ssh
# connection. In certain cases `make testall` will be faster than `ctest -j4` because of fewer
# instantiations of QApplication.

add_custom_target(testall
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testmvvm_view
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testmvvm_integration
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testmvvm_viewmodel
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testmvvm_model
)
