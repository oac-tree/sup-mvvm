set(library_name test-utils)

add_library(${library_name} STATIC "")

add_subdirectory(mvvm)

target_link_libraries(${library_name} PUBLIC sup-mvvm::test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<BUILD_INTERFACE:${SUP_MVVM_AUTOGEN_DIR}>)
