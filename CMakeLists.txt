cmake_minimum_required(VERSION 3.10)

project(example-cpp-gui-license-validation)

find_package(Qt5 REQUIRED
  COMPONENTS Widgets Network
  PATHS (
    "../qt/lib/cmake/Qt5"
    "/usr/local/opt/qt@5/lib/cmake/Qt5"
  )
)

# Tell CMake to run moc (a Qt tool) when necessary.
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(MSVC)
  # Do not open a cmd window.
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

add_executable(example-cpp-gui-license-validation
  src/main.cpp
  src/main_widget.cpp
  src/main_widget.h
)

target_compile_features(example-cpp-gui-license-validation PUBLIC cxx_std_11)

target_link_libraries(example-cpp-gui-license-validation
  Qt5::Widgets
  Qt5::Network
)

if(WIN32)
  # After every build, copy Qt5 DLLs if needed. OpenSSL 1.0.2 is required for
  # HTTPS web requests, but the OpenSSL DLL files must be made available manually.
  add_custom_command(
    TARGET example-cpp-gui-license-validation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:Qt5::Core>
      $<TARGET_FILE:Qt5::Gui>
      $<TARGET_FILE:Qt5::Widgets>
      $<TARGET_FILE:Qt5::Network>
      $<TARGET_FILE_DIR:example-cpp-gui-license-validation>
    COMMAND ${CMAKE_COMMAND} -E make_directory
      "$<TARGET_FILE_DIR:example-cpp-gui-license-validation>/platforms/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>
      "$<TARGET_FILE_DIR:example-cpp-gui-license-validation>/platforms/"
  )
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PROPERTY VS_STARTUP_PROJECT example-cpp-gui-license-validation
)
