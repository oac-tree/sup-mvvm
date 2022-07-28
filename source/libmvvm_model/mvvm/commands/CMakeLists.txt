target_sources(${library_name} PRIVATE
  abstract_command.cpp
  abstract_command.h
  command_interface.h
  command_model_composer.cpp
  command_model_composer.h
  command_stack.cpp
  command_stack.h
  command_stack_interface.h
  remove_item_command.cpp
  remove_item_command.h
  set_value_command.cpp
  set_value_command.h
  )
