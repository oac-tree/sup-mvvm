# Setup target for documentation generation.

# --- Finding Doxygen ---
find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
  message(INFO "No doxygen found, skipping documentation generation")
  return()
endif()

# --- Finding Sphinx ---
find_package(Sphinx)
if(NOT Sphinx_FOUND)
  message(INFO "No Sphinx found, skipping documentation generation")
  return()
endif()

# just output XML since this is what Breathe consumes
set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_XML YES)

# Note: use doxygen_add_docs(doxygen-doc ALL ...) if you want your documentation
# to be created by default each time you build. Without the keyword you need to
# explicitly invoke building of the 'doc' target.
doxygen_add_docs(doxygen-doc ${PROJECT_SOURCE_DIR}/source/libmvvm_model COMMENT
                 "Generating API documentation with Doxygen")

configure_file(${SUP_MVVM_PROJECT_DIR}/cmake/configs/conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/conf.py @ONLY)
add_custom_target(sphinx-doc
  COMMAND ${SPHINX_EXECUTABLE} -b html -c ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating API documentation with Sphinx"
  VERBATIM)

 add_dependencies(sphinx-doc doxygen-doc)

