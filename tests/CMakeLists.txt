enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

find_package(Qt5 COMPONENTS Widgets Core Test REQUIRED)
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
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testmvvm_view
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testmvvm_integration
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testmvvm_viewmodel
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testmvvm_model
)
