cmake_minimum_required(VERSION 3.15)
project(sca)

# check if we have eigen3 installed in the system
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)

# add the include directories
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(include)

# add the executable
add_executable(sca main.cpp
        src/cpa.cpp
        include/cpa.hpp)