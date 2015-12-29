project(eigenml)
cmake_minimum_required(VERSION 2.8)

# set some global options
set(BOOST_VERSION 1.59.0)
set(optimization "-O2")
set(warnings "-Wall -Wextra") # introduce -Werror for release build
set(c_11 "-g -std=c++11 -pedantic")
set(CMAKE_CXX_FLAGS "${opimization} ${warnings} ${c_11}")


# add some general include directories
# for src/ and test/
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("/usr/local/include/eigen3")

# set dynamic linking for boost
add_definitions(-DBOOST_LOG_DYN_LINK)

# configure packages for test
enable_testing()

# compile the different modules
add_subdirectory(src)

# compile the tests
add_subdirectory(test)
