# tests for all classes from sup-mvvm-viewmodel

set(test testsup-mvvm-viewmodel)

add_executable(${test})

target_sources(${test} PRIVATE
  abstract_editor_factory_tests.cpp
  abstract_presentation_item_tests.cpp
  abstract_row_strategy_tests.cpp
  abstract_viewmodel_controller_tests.cpp
  all_items_viewmodel_tests.cpp
  boolean_data_presentation_item_tests.cpp
  chart_axis_plot_controller_tests.cpp
  chart_axis_title_controller_tests.cpp
  connectable_shape_style_tests.cpp
  custom_variants_tests.cpp
  default_data_presentation_item_tests.cpp
  default_display_name_presentation_item_tests.cpp
  filter_name_viewmodel_tests.cpp
  item_view_map_tests.cpp
  line_series_controller_tests.cpp
  line_series_data_controller_tests.cpp
  main.cpp
  mvvm_style_helper_tests.cpp
  node_connection_shape_tests.cpp
  node_connection_style_tests.cpp
  node_connection_tests.cpp
  node_editor_events_tests.cpp
  node_editor_helper_tests.cpp
  node_item_connectable_shape_adapter_tests.cpp
  node_port_style_tests.cpp
  node_port_tests.cpp
  painter_helper_tests.cpp
  platform_tests.cpp
  port_appearance_tests.cpp
  port_info_test.cpp
  presentation_item_helper_tests.cpp
  property_table_viewmodel_tests.cpp
  property_viewmodel_tests.cpp
  qtcore_helper_tests.cpp
  standard_children_strategies_tests.cpp
  standard_presentation_items_tests.cpp
  standard_row_strategies_tests.cpp
  top_bottom_port_position_strategy_tests.cpp
  top_items_viewmodel_tests.cpp
  variant_converter_tests.cpp
  variant_display_role_visitor_tests.cpp
  viewitem_data_tests.cpp
  viewitem_factory_tests.cpp
  viewitem_tests.cpp
  viewmodel_base_tests.cpp
  viewmodel_controller_factory_tests.cpp
  viewmodel_controller_tests.cpp
  viewmodel_iterate_helper_tests.cpp
  viewmodel_tests.cpp
  viewmodel_utils_tests.cpp
)

target_link_libraries(${test} PRIVATE sup-mvvm::test sup-mvvm-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST TEST_PREFIX "ViewModel.")
