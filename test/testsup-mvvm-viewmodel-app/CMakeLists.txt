# tests for all classes from sup-mvvm-viewmodel (that require QApplication)

set(test testsup-mvvm-viewmodel-app)

add_executable(${test} "")

target_sources(${test} PRIVATE
    chart_pen_controller_tests.cpp
    chart_viewport_controller_tests.cpp
    customplot
    custom_viewmodel_tests.cpp
    main.cpp
)

if(SUP_MVVM_BUILD_QCUSTOMPLOT)
  add_subdirectory(customplot)
endif()

target_link_libraries(${test} PRIVATE sup-mvvm::test test-utils)
if (SUP_MVVM_BUILD_QCUSTOMPLOT)
  target_link_libraries(${test} PRIVATE qcustomplot)
endif()

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} TEST_PREFIX "Integration." DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST)

