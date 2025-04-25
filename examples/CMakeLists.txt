if(NOT SUP_MVVM_BUILD_EXAMPLES)
  return()
endif()

add_subdirectory(celleditors)
add_subdirectory(collidingmice)
add_subdirectory(customeditors)
add_subdirectory(dragandmove)
add_subdirectory(flatview)
add_subdirectory(hello-world)
add_subdirectory(lineseries)
add_subdirectory(saveloadproject)

if (SUP_MVVM_BUILD_QCUSTOMPLOT)
  add_subdirectory(concurrentplot)
  add_subdirectory(plotgraphs)
endif()

install(DIRECTORY ${SUP_MVVM_PROJECT_DIR}/examples DESTINATION share/mvvm)

