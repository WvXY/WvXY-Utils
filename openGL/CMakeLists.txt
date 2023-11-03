cmake_minimum_required(VERSION 3.22)
project(OpenGL_Demo)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(glad/)

#find_package(Eigen3 CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

#find_package(CGAL REQUIRED)
#find_package(Boost REQUIRED)
#find_package(GMP REQUIRED)
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)

add_executable(${PROJECT_NAME}
#        demo_legacy.cpp
        demo_modern.cpp
        )

target_link_libraries(${PROJECT_NAME}
#        PkgConfig::gmp
#        Eigen3::Eigen
        OpenGL::GL
        glfw
        glad
#        CGAL
        )