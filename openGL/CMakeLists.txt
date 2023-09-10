cmake_minimum_required(VERSION 3.22)
project(OpenGL Demo)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CGAL REQUIRED)
find_package(Eigen3 REQUIRED)
#find_package(GMP REQUIRED)

add_subdirectory(glad/)
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)


add_executable(${PROJECT_NAME}
        main.cpp
        )

target_link_libraries(${PROJECT_NAME}
#        PkgConfig::gmp
#        Eigen3
        OpenGL::GL
        glfw
        glad
        CGAL
        )