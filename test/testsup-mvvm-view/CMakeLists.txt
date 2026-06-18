# tests for all classes from sup-mvvm-view (require QApplication)

set(test testsup-mvvm-view)

add_executable(${test})

target_sources(${test} PRIVATE
  abstract_item_view_tests.cpp
  abstract_project_user_interactor_tests.cpp
  allint_spinbox_tests.cpp
  all_items_tree_view_tests.cpp
  chart_view_tests.cpp
  combo_property_editor_tests.cpp
  default_cell_decorator_tests.cpp
  default_editor_factory_tests.cpp
  float_spinbox_tests.cpp
  item_view_component_provider_tests.cpp
  main.cpp
  property_flat_view_tests.cpp
  property_grid_controller_tests.cpp
  property_tree_view_tests.cpp
  recent_project_path_tests.cpp
  role_dependent_editor_factory_tests.cpp
  scientific_spinbox_tests.cpp
  selectable_combobox_editor_tests.cpp
  string_completer_combo_editor_tests.cpp
  string_completer_editor_tests.cpp
  top_items_tree_view_tests.cpp
  variant_dependent_editor_factory_tests.cpp
  viewmodel_component_builder_tests.cpp
  viewmodel_delegate_extended_tests.cpp
  viewmodel_delegate_tests.cpp
  widget_utils_tests.cpp
)

target_link_libraries(${test} PRIVATE sup-mvvm::view sup-mvvm::test sup-mvvm-test-utils)
if (SUP_MVVM_BUILD_QCUSTOMPLOT)
  target_link_libraries(${test} PRIVATE qcustomplot)
endif()

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST TEST_PREFIX "View.")
