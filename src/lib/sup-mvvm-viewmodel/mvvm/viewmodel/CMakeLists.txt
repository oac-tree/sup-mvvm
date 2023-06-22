# ViewModel to represent SessionItem and SessionModel.

target_sources(${library_name} PRIVATE
  abstract_viewmodel_controller.cpp
  abstract_viewmodel_controller.h
  all_items_viewmodel.cpp
  all_items_viewmodel.h
  custom_variants.cpp
  custom_variants.h
  property_table_viewmodel.cpp
  property_table_viewmodel.h
  property_viewmodel.cpp
  property_viewmodel.h
  standard_children_strategies.cpp
  standard_children_strategies.h
  standard_presentation_items.cpp
  standard_presentation_items.h
  standard_row_strategies.cpp
  standard_row_strategies.h
  top_items_viewmodel.cpp
  top_items_viewmodel.h
  variant_converter.cpp
  variant_converter.h
  viewitem_factory.cpp
  viewitem_factory.h
  viewitem_map.cpp
  viewitem_map.h
  viewmodel.cpp
  viewmodel.h
  viewmodel_controller.cpp
  viewmodel_controller.h
  viewmodel_controller_impl.cpp
  viewmodel_controller_impl.h
  viewmodel_controller_old_impl.cpp
  viewmodel_controller_old_impl.h
  viewmodel_utils.cpp
  viewmodel_utils.h
  )
