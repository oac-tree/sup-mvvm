add_subdirectory(hello-world)
add_subdirectory(toy-psps-model)

# installing only single example
install(DIRECTORY ${CMAKE_SOURCE_DIR}/examples DESTINATION share/mvvm)

