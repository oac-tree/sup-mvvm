# Experimental code to try out various refactoring solutions. Not intended for production.

target_sources(${library_name} PRIVATE
  cloneable.h
  light_item.cpp
  light_item.h
  light_item_impl.cpp
  light_item_impl.h
  light_model.cpp
  light_model.h
  i_light_item.h
  light_set_data_command.cpp
  light_set_data_command.h
)
