set(library_name sup-utils-tree-data)

add_library(${library_name} STATIC "")

target_include_directories(${library_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_sources(${library_name}
  PRIVATE
    exceptions.cpp
    tree_data_parser.cpp
    tree_data_parser_utils.cpp
    tree_data_serialize.cpp
    tree_data_serialize_utils.cpp
    tree_data_validate.cpp
    tree_data.cpp
    xml_utils.cpp
)

target_link_libraries(${library_name} PRIVATE LibXml2::LibXml2)
