CMAKE_MINIMUM_REQUIRED(VERSION 3.26)
project(WVXY_Cpp_Utils)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

add_subdirectory(OpenGL_Utils)  # not working now at this scope
# TODO: shader folder location and compiled file locations