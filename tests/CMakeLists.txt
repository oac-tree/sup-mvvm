enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

find_package(Qt5 COMPONENTS Widgets Core Test REQUIRED)
find_package(Threads)

add_subdirectory(libtestmachinery)
add_subdirectory(testmvvm_model)
add_subdirectory(testmvvm_viewmodel)
