# Operational Applications UI foundation

Collection of C++/Qt common components for Operational Application User
Interfaces.

## Requirements

- C++17
- CMake 3.14
- Qt 5.12
- libxml2

## Installation

### Installation on Linux of the user's choice

```bash
cmake <path-to-repo> && make -j4 && ctest
```

### Installation on CODAC machines

Qt must be installed, and path to it specified.

```
# install latest gtest, libxml2
yum install codac-core-7.1-gtest-1.12.1-devel.x86_64 libxml2-devel.x86_64

# install qt5
yum install qt5-qtbase-devel.x86_64  \
            qt5-qtsvg-devel.x86_64   \
            qt5-qttools-devel.x86_64 \
            qt5-qtbase-gui.x86_64    \
            adwaita-qt5.x86_64       \
            mesa-libGL-devel.x86_64

# specify path to the repo directory  and compile
mkdir build; cd build
cmake -DCMAKE_PREFIX_PATH=/opt/codac/common/gtest-1.12.1 <path-to-repo>
make -j4 && ctest
```

Please note, that thanks to CMake magic (the magic is located in $HOME/.cmake
directory), `make install` step is optional. Without installation, libraries
will be discoverable right from the build directory.
