add_subdirectory(celleditors)
add_subdirectory(hello-world)
add_subdirectory(plotgraphs)
add_subdirectory(dragandmove)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/examples DESTINATION share/mvvm)

