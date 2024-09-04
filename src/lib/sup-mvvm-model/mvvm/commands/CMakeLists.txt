target_sources(${library_name} PRIVATE
  abstract_command.cpp
  abstract_command.h
  command_model_composer.cpp
  command_model_composer.h
  command_stack.cpp
  command_stack.h
  i_command.h
  i_command_stack.h
  insert_item_command.cpp
  insert_item_command.h
  macro_command.cpp
  macro_command.h
  remove_item_command.cpp
  remove_item_command.h
  set_value_command.cpp
  set_value_command.h
  )
