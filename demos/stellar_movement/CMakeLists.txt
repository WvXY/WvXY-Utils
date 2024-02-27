cmake_minimum_required(VERSION 3.26)
project(Stellar_Movement)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

find_package(Eigen3 CONFIG REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
        stellar.h
)

target_link_libraries(${PROJECT_NAME}
        Eigen3::Eigen
        glfw
        OpenGL::GL
)