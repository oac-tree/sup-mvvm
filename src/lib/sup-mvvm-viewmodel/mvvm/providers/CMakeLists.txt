# Various components for viewmodel
# Doesn't contain any QObject based classes, included in Sonar analysis

target_sources(${library_name} PRIVATE
  abstract_editor_factory.cpp
  abstract_editor_factory.h
  abstract_row_strategy.cpp
  abstract_row_strategy.h
  abstract_viewmodel_controller.cpp
  abstract_viewmodel_controller.h
  custom_variants.cpp
  custom_variants.h
  editor_types_fwd.h
  i_cell_decorator.h
  i_children_strategy.h
  i_editor_factory.h
  i_row_strategy.h
  i_viewitem_data.h
  i_viewmodel_controller.h
  qtcore_helper.cpp
  qtcore_helper.h
  standard_children_strategies.cpp
  standard_children_strategies.h
  standard_presentation_items.cpp
  standard_presentation_items.h
  standard_row_strategies.cpp
  standard_row_strategies.h
  variant_converter.cpp
  variant_converter.h
  viewitem.cpp
  viewitem_data.cpp
  viewitem_data.h
  viewitem_factory.cpp
  viewitem_factory.h
  viewitem.h
  viewmodel_controller.cpp
  viewmodel_controller_factory.cpp
  viewmodel_controller_factory.h
  viewmodel_controller.h
  viewmodel_controller_impl.cpp
  viewmodel_controller_impl.h
  viewmodel_iterate_helper.cpp
  viewmodel_iterate_helper.h
  viewmodel_utils.cpp
  viewmodel_utils.h
  item_view_map.h
  )
