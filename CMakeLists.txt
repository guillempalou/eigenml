project(eigenml)
cmake_minimum_required(VERSION 2.8)

# set some options
set(optimization "-O2")
set(warnings "-Wall -Wextra -Werror")
set(c_11 "-g -std=c++11 -pedantic")
set(CMAKE_CXX_FLAGS "${opimization} ${warnings} ${c_11}")

# add some include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("/usr/local/include/eigen3")

# configure packages for test
enable_testing()

# add the tests
add_subdirectory(test)

# configure an example to run
add_executable(eigenml src/examples/main.cpp)