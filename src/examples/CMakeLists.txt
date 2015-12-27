file(GLOB_RECURSE SRCS *.cpp)

add_executable(main_example ${SRCS})

target_link_libraries(main_example eigenml_logging)