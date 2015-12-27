file(GLOB_RECURSE SRCS *.cpp)

add_library(eigenml_decision_tree ${SRCS})

target_link_libraries(eigenml_decision_tree eigenml_core)