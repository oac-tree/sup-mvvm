############################################################################
# CMakeLists.txt file for building qcustomplot static library
############################################################################
set(library_name qcustomplot)

set(CMAKE_AUTOMOC ON)
add_library(${library_name} STATIC qcustomplot.h qcustomplot.cpp)
set_property(TARGET qcustomplot PROPERTY POSITION_INDEPENDENT_CODE ON)
# Q_ENUMS and Q_FLAGS are deprecated in Qt6
target_compile_options(${library_name} PRIVATE
  $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wno-deprecated-pragma>)
target_link_libraries(${library_name} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets  Qt${QT_VERSION_MAJOR}::PrintSupport)
target_include_directories(${library_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
