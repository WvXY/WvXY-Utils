cmake_minimum_required(VERSION 3.6)
project(OpenGLDemo)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad)

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
#        demo_modern.cpp
        gl_utils.cpp
        gl_utils.hpp
        main.cpp
        )

target_link_libraries(${PROJECT_NAME}
#        PkgConfig::gmp
#        Eigen3::Eigen
        OpenGL::GL
        glfw
        glad
#        CGAL
        )