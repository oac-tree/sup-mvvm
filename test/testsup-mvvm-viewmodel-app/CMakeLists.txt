# tests for all classes from sup-mvvm-viewmodel (that require QApplication)

set(test testsup-mvvm-viewmodel-app)

add_executable(${test} "")

target_sources(${test} PRIVATE
  allint_spinbox_tests.cpp
  chart_pen_controller_tests.cpp
  chart_viewport_controller_tests.cpp
  custom_editor_factories_tests.cpp
  custom_viewmodel_tests.cpp
  float_spinbox_tests.cpp
  item_view_component_provider_tests.cpp
  main.cpp
  property_grid_controller_tests.cpp
  viewmodel_delegate_tests.cpp
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

