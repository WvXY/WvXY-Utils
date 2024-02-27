CMAKE_MINIMUM_REQUIRED(VERSION 3.26)
project(WvXY_Utils)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

add_subdirectory(openGL)
add_subdirectory(meshIO)
add_subdirectory(demos/stellar_movement)
