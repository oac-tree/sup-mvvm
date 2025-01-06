if(NOT SUP_MVVM_BUILD_EXAMPLES)
  return()
endif()

add_subdirectory(celleditors)
add_subdirectory(collidingmice)
add_subdirectory(concurrentplot)
add_subdirectory(dragandmove)
add_subdirectory(flatview)
add_subdirectory(hello-world)
add_subdirectory(lineseries)
add_subdirectory(plotgraphs)
add_subdirectory(saveloadproject)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/examples DESTINATION share/mvvm)

