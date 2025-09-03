# NodeEditor classes
# Folder contains QObject derived classes and is excluded from Sonar analysis

target_sources(${library_name} PRIVATE
  i_node_connection.h
  i_node_port.h
  node_connection.cpp
  node_connection.h
  node_connection_shape.cpp
  node_connection_shape.h
  node_port.cpp
  node_port.h
  node_port_shape.cpp
  node_port_shape.h
  port_info.cpp
  port_info.h
  node_editor_types.h
  )
