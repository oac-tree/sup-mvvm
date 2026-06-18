# benchmark tests

set(test testsup-mvvm-benchmark)

add_executable(${test})

target_sources(${test} PRIVATE
  abstract_viewmodel_controller_benchmark.cpp
  all_items_viewmodel_benchmark.cpp
  application_model_benchmark.cpp
  item_backup_strategy_benchmark.cpp
  main.cpp
  model_event_handler_benchmark.cpp
  model_utils_benchmark.cpp
  sessionitem_benchmark.cpp
  sessionmodel_benchmark.cpp
  unique_id_generator_performance.cpp
)

target_link_libraries(${test} PRIVATE sup-mvvm::test benchmark::benchmark sup-mvvm-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})
