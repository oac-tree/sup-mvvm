# tests for all classes from sup-mvvm-viewmodel (that require QApplication)

set(test testsup-mvvm-viewmodel-app)

add_executable(${test} "")

target_sources(${test} PRIVATE
  allint_spinbox_tests.cpp
  chart_pen_controller_tests.cpp
  chart_viewport_controller_tests.cpp
  combo_property_editor_tests.cpp
  custom_viewmodel_tests.cpp
  default_editor_factory_tests.cpp
  float_spinbox_tests.cpp
  main.cpp
  role_dependent_editor_factory_tests.cpp
  selectable_combobox_editor_tests.cpp
  variant_dependent_editor_factory_tests.cpp
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

