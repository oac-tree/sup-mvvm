set(library_name testmachinery)

add_library(${library_name} STATIC "")

add_subdirectory(testutils)

target_link_libraries(${library_name} PUBLIC pthread GTest::gtest GTest::gmock sup-mvvm::view Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<BUILD_INTERFACE:${SUP_MVVM_AUTOGEN_DIR}>)
