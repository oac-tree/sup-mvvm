set(library_name sup-utils-tree-data)

add_library(${library_name} STATIC "")

target_include_directories(${library_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_sources(${library_name} PRIVATE
  exceptions.cpp
  exceptions.h
  tree_data_parser.cpp
  tree_data_parser.h
  tree_data_parser_utils.cpp
  tree_data_parser_utils.h
  tree_data_serialize.cpp
  tree_data_serialize.h
  tree_data_serialize_utils.cpp
  tree_data_serialize_utils.h
  tree_data_validate.cpp
  tree_data_validate.h
  tree_data.cpp
  tree_data.h
  xml_utils.cpp
  xml_utils.h
)

target_link_libraries(${library_name} PRIVATE LibXml2::LibXml2)
