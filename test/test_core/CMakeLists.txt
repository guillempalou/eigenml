file(GLOB_RECURSE SRCS *.cpp)

add_executable(test_vectors ${SRCS})

target_link_libraries(test_vectors
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    eigenml_core
    eigenml_logging
    # for now we don't need it ${GMOCK_LIBRARIES}
)

# add itself to the tests
add_test(test_core test_vectors)