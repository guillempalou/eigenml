file(GLOB SRCS *.cpp)

add_library(eigenml_core ${SRCS})

target_link_libraries(eigenml_core eigenml_logging)