# Experimental code to try out various refactoring solutions. Not intended for production.

target_sources(${library_name} PRIVATE
  abstract_model_decorator.cpp
  abstract_model_decorator.h
  item.cpp
  item.h
  model.cpp
  model.h
  model_interface.h
  reporting_model.cpp
  reporting_model.h
  reporting_model_v2.cpp
  reporting_model_v2.h
  undoable_model.cpp
  undoable_model.h
)
