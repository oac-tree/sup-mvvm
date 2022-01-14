set(library_name stduuid)

add_library(${library_name} INTERFACE)
target_include_directories(${library_name} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gsl>)
