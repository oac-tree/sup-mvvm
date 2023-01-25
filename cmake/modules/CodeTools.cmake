# Collection of functions to set-up code beautification and analysis
include(ClangFormat)

# List of targets for project code beautification.
set(BEAUTIFICATION_TARGETS sup-mvvm-model sup-mvvm-viewmodel sup-mvvm-view testmvvm_model testmvvm_viewmodel testmvvm_view testmvvm_integration)
set(BEAUTIFICATION_EXAMPLES celleditorscore plotgraphscore concurrentplotcore dragandmovecore flatviewcore)

# Defines new target for 'clangformat' to beautify whole project.
# Use 'make clangformat' or 'cmake --build . --target clangformat' to beautify the code.
# Beautification settings are located in .clang-format in project directory.

function(project_clangformat_setup)
    set(all_sources)
    foreach(target ${BEAUTIFICATION_TARGETS})
        get_target_property(target_sources ${target} SOURCES)
        list(APPEND all_sources ${target_sources})
    endforeach()
    # examples needs to add manualy target source dir to all names
    foreach(target ${BEAUTIFICATION_EXAMPLES})
        get_target_property(target_sources ${target} SOURCES)
        get_target_property(target_source_dir ${target} SOURCE_DIR)
        foreach(target ${target_sources})
            list(APPEND all_sources ${target_source_dir}/${target})
        endforeach()
    endforeach()
   clangformat_setup(${all_sources})
endfunction()


# Setups test coverage target.
# Use 'make codecoverage' or 'cmake --build . --target coverage' to generate coverage report.
# Requires -DSUP_MVVM_GENERATE_COVERAGE=ON

function(project_testcoverage_setup)
    setup_target_for_coverage_lcov(NAME coverage
        EXECUTABLE ctest -j 4
        DEPENDENCIES testmvvm_model testmvvm_viewmodel testmvvm_view testmvvm_integration
        EXCLUDE
            "${PROJECT_SOURCE_DIR}/EXTERNAL_OBJECT/*"
            "${PROJECT_SOURCE_DIR}/tests/*"
            "${PROJECT_SOURCE_DIR}/examples/*"
            "${PROJECT_SOURCE_DIR}/source/libmvvm_view/*"
            "**CompilerId*" "/usr/*"  "${CMAKE_BINARY_DIR}/*"
        )
endfunction()

# Setups targets for code processing.

function(project_codetools_setup)
    if (SUP_MVVM_SETUP_CLANGFORMAT)
        project_clangformat_setup()
    endif()
    if (SUP_MVVM_SETUP_COVERAGE)
        project_testcoverage_setup()
    endif()
endfunction()

function(fetch_googletest)
  message(STATUS "GTest with version >= \"1.10\" was not found, fetching from internet" )
  include(FetchContent)

  # version 1.12.1
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850)
#  FetchContent_Declare(
#    googletest
#    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
#  )

FetchContent_MakeAvailable(googletest)
#    FetchContent_GetProperties(googletest)

#    if(NOT googletest_POPULATED)
#      FetchContent_Populate(googletest)
#      add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
#    endif()



  message(STATUS "GTest binaries are present at ${googletest_BINARY_DIR}")
endfunction()
