# Components to provide Qt views with necessary components (selection models, delegates, editors)

target_sources(${library_name} PRIVATE
  abstract_editor_factory.cpp
  abstract_editor_factory.h
  custom_editor.cpp
  custom_editor.h
  i_cell_decorator.h
  i_editor_factory.h
  )
