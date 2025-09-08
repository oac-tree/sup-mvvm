# NodeEditor classes
# Folder contains QObject derived classes and is excluded from Sonar analysis

target_sources(${library_name} PRIVATE
  connectable_shape.cpp
  connectable_shape.h
  i_connectable_shape_adapter.h
  i_node_connection.h
  i_node_port.h
  i_port_position_strategy.h
  node_connection.cpp
  node_connection.h
  node_connection_shape.cpp
  node_connection_shape.h
  node_editor_types.h
  node_port.cpp
  node_port.h
  node_port_shape.cpp
  node_port_shape.h
  port_appearance.cpp
  port_appearance.h
  port_info.cpp
  port_info.h
  top_bottom_port_position_strategy.cpp
  top_bottom_port_position_strategy.h
  )
