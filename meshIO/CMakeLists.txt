cmake_minimum_required(VERSION 3.10)
project(data_io_test)

set(CMAKE_CXX_STANDARD 17)

# https://stackoverflow.com/questions/41361631/optimize-in-cmake-by-default
 if(NOT CMAKE_BUILD_TYPE)
     set(CMAKE_BUILD_TYPE Release)
 endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Eigen3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
        data_io.cpp
        data_io.hpp
        data_io_test.cpp
)

target_link_libraries(${PROJECT_NAME}
        Eigen3::Eigen
)



