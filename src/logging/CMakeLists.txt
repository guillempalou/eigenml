# find logging packages
find_package(Threads)
find_package(Boost ${BOOST_VERSION} COMPONENTS system log log_setup thread REQUIRED)

# find the cpp
file(GLOB_RECURSE SRCS *.cpp)
add_library(eigenml_logging ${SRCS})

# find the boost log library
if(Boost_FOUND)
  # include the boost directories
  include_directories( ${Boost_INCLUDE_DIRS} )
  target_link_libraries(eigenml_logging ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
else(Boost_FOUND)
  message(FATAL_ERROR "Cannot build without Boost. Please set Boost_DIR.")
endif(Boost_FOUND)

