# Components for Qt models (selection models, delegates)

target_sources(${library_name} PRIVATE
  default_cell_decorator.cpp
  default_cell_decorator.h
  item_selection_model.cpp
  item_selection_model.h
  item_view_component_provider.cpp
  item_view_component_provider.h
  viewmodel_delegate.cpp
  viewmodel_delegate.h
  )
