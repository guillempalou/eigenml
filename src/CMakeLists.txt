# add some general include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("/usr/local/include/eigen3")

# compile the logging module
add_subdirectory(logging)

# compile the examples
add_subdirectory(examples)

