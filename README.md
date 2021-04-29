# example-cpp-gui-license-validation

[![Build Status](https://github.com/keygen-sh/example-cpp-gui-license-validation/workflows/test/badge.svg)](https://github.com/keygen-sh/example-cpp-gui-license-validation/actions)

This is an example C++ GUI program for validating a license key. For convenience, the source code contains a demo account id and license key.

The example app has been tested on Windows, Mac and Ubuntu.

### Requirements

* CMake 3.10 or later
* Qt 5
* OpenSSL 1.0.2

### CMake

The [CMake file](CMakeLists.txt) for this example project does not automatically download Qt 5 or OpenSSL. The headers and libs for Qt 5 are expected to already exist on the local machine. Qt 5 in turn requires OpenSSL 1.0.2 (exact version) libs for HTTPS requests.

The call to *find_package* contain hints on where to find Qt 5 (in a sibling folder to the example project or at the default installation path when installed with [Homebrew](https://brew.sh/) on a Mac) in addition to the default paths where CMake will look for it. If all else fails you will need to declare a value for `Qt5_DIR` for CMake. Example:

```bash
cmake \
  -DQt5_DIR="C:/folderA/qt/lib/cmake/Qt5" \
  -DCMAKE_GENERATOR_PLATFORM=x64 \
  ..
```

### Windows DLL files

On Windows you will typically want to copy the necessary DLL files to the same folder as your EXE file, so that you can run your application. In order to do that automatically after every build (but only if needed), the [CMake file](CMakeLists.txt) contains this:

```cmake
if(WIN32)
  # After every build, copy Qt5 DLLs if needed.
  add_custom_command(
    TARGET HjsonExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:Qt5::Core>
      $<TARGET_FILE:Qt5::Gui>
      $<TARGET_FILE:Qt5::Widgets>
      $<TARGET_FILE_DIR:HjsonExample>
    COMMAND ${CMAKE_COMMAND} -E make_directory
      "$<TARGET_FILE_DIR:HjsonExample>/platforms/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>
      "$<TARGET_FILE_DIR:HjsonExample>/platforms/"
  )
endif()
```

However, OpenSSL DLL files (libeay32.dll and ssleay32.dll) are not automatically copied.
