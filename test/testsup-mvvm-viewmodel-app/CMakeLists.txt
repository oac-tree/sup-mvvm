# tests for all classes from sup-mvvm-viewmodel (that require QApplication)

set(test testsup-mvvm-viewmodel-app)

add_executable(${test} "")

target_sources(${test} PRIVATE
    chart_pen_controller_tests.cpp
    chart_viewport_controller_tests.cpp
    connectable_shape_extended_tests.cpp
    connectable_shape_painter_tests.cpp
    connectable_shape_tests.cpp
    connectable_view_model_controller_tests.cpp
    custom_viewmodel_tests.cpp
    graphics_scene_helper_tests.cpp
    line_series_mouse_listener_tests.cpp
    main.cpp
    node_connection_guide_impl_tests.cpp
    node_connection_guide_tests.cpp
    node_connection_painter_tests.cpp
    node_port_shape_tests.cpp
    custom_label_viewmodel_tests.cpp
)

if(SUP_MVVM_BUILD_QCUSTOMPLOT)
  add_subdirectory(customplot)
endif()

target_link_libraries(${test} PRIVATE sup-mvvm::test sup-mvvm-test-utils)
if (SUP_MVVM_BUILD_QCUSTOMPLOT)
  target_link_libraries(${test} PRIVATE qcustomplot)
endif()

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} TEST_PREFIX "Integration." DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST)

