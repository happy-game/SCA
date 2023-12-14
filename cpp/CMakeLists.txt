cmake_minimum_required(VERSION 3.15)
project(sca)

# check if we have eigen3 installed in the system
find_package(Eigen3 REQUIRED)
find_package(GSL REQUIRED)
find_package(RapidJSON REQUIRED)

# add the include directories
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${GSL_INCLUDE_DIRS})
include_directories(${RAPIDJSON_INCLUDE_DIRS})
include_directories(include)

# src/ directory and include/ directory
FILE(GLOB_RECURSE SRC_FILES src/*.cpp)
FILE(GLOB_RECURSE INC_FILES include/*.hpp)

# add the executable
add_executable(sca ${SRC_FILES} ${INC_FILES} main.cpp )
target_link_libraries(sca ${GSL_LIBRARIES})