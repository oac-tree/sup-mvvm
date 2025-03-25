target_sources(${library_name} PRIVATE
  abstract_item_view.cpp
  abstract_item_view.h
  all_items_tree_view.cpp
  all_items_tree_view.h
  chart_canvas.cpp
  chart_canvas.h
  chart_view.cpp
  chart_view.h
  component_provider_helper.cpp
  component_provider_helper.h
  property_flat_view.cpp
  property_flat_view.h
  property_tree_view.cpp
  property_tree_view.h
  top_items_tree_view.cpp
  top_items_tree_view.h
)

if (SUP_MVVM_BUILD_QCUSTOMPLOT)
target_sources(${library_name} PRIVATE
  graph_canvas.cpp
  graph_canvas.h
)
endif()

