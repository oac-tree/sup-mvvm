# Components to provide Qt views with necessary components (selection models, delegates, editors)

target_sources(${library_name} PRIVATE
  abstract_editor_factory.cpp
  abstract_editor_factory.h
  default_cell_decorator.cpp
  default_cell_decorator.h
  i_cell_decorator.h
  i_editor_factory.h
  item_view_component_provider.cpp
  item_view_component_provider.h
  property_grid_controller.cpp
  property_grid_controller.h
  viewmodel_delegate.cpp
  viewmodel_delegate.h
  )
