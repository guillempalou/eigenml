find_package(GTest REQUIRED)
#find_package(GMock REQUIRED)
find_package(Threads)

include_directories(${GTEST_INCLUDE_DIRS})
#include_directories(${GMOCK_INCLUDE_DIRS})

add_subdirectory(test_tree)