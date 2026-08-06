# -----------------------------------------------------------------------------
# Installation
# Credits to https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right,
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/Exporting-and-Importing-Targets
# -----------------------------------------------------------------------------
if (COA_WEB_ASSEMBLY)
  return()
endif()

include(PackageConfig)
include(CMakePackageConfigHelpers)

set(BUILD_CONFIGDIR ${CMAKE_CURRENT_BINARY_DIR})
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/sup-mvvm)

# -----------------------------------------------------------------------------
# Exporting targets
# -----------------------------------------------------------------------------
set(PACKAGE_TARGETS_FILENAME sup-mvvm-targets.cmake)
set(PACKAGE_TARGETS_FILE ${BUILD_CONFIGDIR}/${PACKAGE_TARGETS_FILENAME})

# Install the export targets for installation usage, this does not relate to the build tree file
install(EXPORT sup-mvvm-targets FILE ${PACKAGE_TARGETS_FILENAME} NAMESPACE sup-mvvm:: DESTINATION ${INSTALL_CONFIGDIR})

# Generate the export targets for the build tree usage
export(TARGETS sup-mvvm-model sup-mvvm-viewmodel sup-mvvm-view NAMESPACE sup-mvvm:: FILE ${PACKAGE_TARGETS_FILE})
if(COA_BUILD_TESTS)
  export(TARGETS sup-mvvm-test NAMESPACE sup-mvvm:: APPEND FILE ${PACKAGE_TARGETS_FILE})
endif()

# Export the package to CMake registry for build tree usage (goes to $HOME/.cmake)
if(COA_EXPORT_BUILD_TREE)
  set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
  export(PACKAGE sup-mvvm)
endif()

# -----------------------------------------------------------------------------
# Version configuration
# -----------------------------------------------------------------------------
set(PACKAGE_VERSION_FILE ${BUILD_CONFIGDIR}/sup-mvvm-config-version.cmake)

# Generate the version config file, shared in both build tree and installation usage
write_basic_package_version_file(${PACKAGE_VERSION_FILE} COMPATIBILITY AnyNewerVersion)

install(FILES ${PACKAGE_VERSION_FILE} DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# Package configuration
# -----------------------------------------------------------------------------
set(PACKAGE_CONFIG_FILE ${BUILD_CONFIGDIR}/sup-mvvm-config.cmake)

if(CODAC_FOUND)
  # Set NO_CMAKE_PATH and PATHS to CMAKE_PREFIX_PATH, so that find_package will use the system Qt first
  # if it finds one, but still look in CMAKE_PREFIX_PATH as a last resort. This gives system Qt priority over CODAC Qt
  set(QT_FIND_OPTIONS NO_CMAKE_PATH PATHS "\$\{CMAKE_PREFIX_PATH\}")
endif()

# Generate the package config file, shared in both build tree and installation usage
write_package_config_file(
  sup-mvvm
  OUTPUT ${PACKAGE_CONFIG_FILE}
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
  DEPENDENCIES Qt${QT_VERSION_MAJOR} GTest LibXml2
  Qt${QT_VERSION_MAJOR}_FIND_OPTIONS ${QT_FIND_OPTIONS} COMPONENTS ${QT_FIND_COMPONENTS}
  GTest_FIND_OPTIONS 1.10)

install(FILES ${PACKAGE_CONFIG_FILE} DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# TarBall
# -----------------------------------------------------------------------------

# Generating the source package
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "sup-mvvm-${LIBVERSION}")

set(CPACK_SOURCE_IGNORE_FILES
    ${CPACK_SOURCE_IGNORE_FILES} # first take the default parameters
    "/\\\\.git/"
    "/\\\\.gitattributes$"
    "/\\\\.github/"
    "/\\\\.gitmodules"
    "/\\\\.gitignore$"
    "/\\\\.tokeignore$"
    "/\\\\.clang-format"
    "/\\\\.clang-tidy"
    "qt-mvvm.code*"
    "CMakeLists.txt.user"
    "build/"
    "/\\\\.ccache"
)

include(CPack)
