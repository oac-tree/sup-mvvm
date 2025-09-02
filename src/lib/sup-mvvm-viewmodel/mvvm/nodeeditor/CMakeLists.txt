# NodeEditor classes
# Folder contains QObject derived classes and is excluded from Sonar analysis

target_sources(${library_name} PRIVATE
  i_node_connection.h
  i_node_port.h
  node_connection.cpp
  node_connection.h
  node_port.cpp
  node_port.h
  port_info.cpp
  port_info.h
  )
