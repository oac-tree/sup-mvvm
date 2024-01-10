Installation
============

Requirements
------------

- C++17
- CMake 3.14
- Qt 5.12
- libxml2

Installation
------------

Installation on CODAC 7.1 machines from RPM
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: sh

  yum install codac-core-7.1-sup-mvvm.x86_64

Installation on Linux of the user's choice
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: sh

  git clone https://git.iter.org/scm/coa/sup-mvvm.git
  mkdir <build>; cd <build>
  cmake <path-to-repo> && make -j4 && ctest


Building on CODAC 7.1 machines from source
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Note: we are using gtest packaged for 7.1 specially.

.. code:: sh

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
   cmake -DCMAKE_PREFIX_PATH=/opt/codac/common/gtest-1.12.1 -DCOA_EXPORT_BUILD_TREE=ON <path-to-repo>
   make -j4 && ctest

Please note, that thanks to CMake magic (the magic is located in
``$HOME/.cmake`` directory), ``make install`` step is optional. Without
installation, libraries will be discoverable right from the build directory.

