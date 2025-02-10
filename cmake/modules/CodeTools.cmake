# Collection of functions to set-up code beautification and analysis
include(ClangFormat)

# List of targets for project code beautification.
set(BEAUTIFICATION_TARGETS sup-mvvm-model sup-mvvm-viewmodel sup-mvvm-view sup-mvvm-test testsup-mvvm-model testsup-mvvm-viewmodel testsup-mvvm-viewmodel-app testsup-mvvm-view testsup-mvvm-benchmark)
set(BEAUTIFICATION_EXAMPLES celleditorscore collidingmicecore concurrentplotcore dragandmovecore flatviewcore lineseriescore plotgraphscore saveloadprojectcore)

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


# Setups targets for code processing.

function(project_codetools_setup)
    if (COA_SETUP_CLANGFORMAT)
        project_clangformat_setup()
    endif()
endfunction()

# Fetches google test version 1.12.1
function(fetch_googletest)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850)
FetchContent_MakeAvailable(googletest)
  message(VERBOSE "GTest binaries are present at ${googletest_BINARY_DIR}")
endfunction()

# --------------------------------------------------------------------------------------------------
# Replacement of add_executable() function that switches to qt_add_executable() when
# COA_WEB_ASSEMBLY flag is set.
# --------------------------------------------------------------------------------------------------
function(coa_add_executable EXEC_NAME)
  cmake_parse_arguments(
      PARSED_ARGS # prefix of output variables
      "" # list of names of the boolean arguments (only defined ones will be true)
      "" # list of names of mono-valued arguments
      "" # list of names of multi-valued arguments (output variables are lists)
      ${ARGN} # arguments of the function to parse, here we take the all original ones
  )
  if (COA_WEB_ASSEMBLY)
    qt_add_executable(${EXEC_NAME} ${ARGN})
  else()
    add_executable(${EXEC_NAME} ${ARGN})
  endif(COA_WEB_ASSEMBLY)
endfunction(coa_add_executable)

# --------------------------------------------------------------------------------------------------
# Replacement of add_library() function that switches to static libs when COA_WEB_ASSEMBLY flag
# is set.
# --------------------------------------------------------------------------------------------------
function(coa_add_library LIB_NAME)
  if (COA_WEB_ASSEMBLY)
    add_library(${LIB_NAME} STATIC "")
  else()
    add_library(${LIB_NAME} SHARED "")
    set_target_properties(${LIB_NAME} PROPERTIES EXPORT_NAME ${export_name} SOVERSION ${SUP_MVVM_SOVERSION} VERSION ${SUP_MVVM_BUILDVERSION})
  endif(COA_WEB_ASSEMBLY)
endfunction(coa_add_library)
