add_subdirectory(hello-world)

# installing only single example
install(DIRECTORY ${CMAKE_SOURCE_DIR}/examples DESTINATION share/mvvm)

