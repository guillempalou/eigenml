file(GLOB SRCS *.cpp)

add_executable(test_tree ${SRCS})

target_link_libraries(test_tree
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    # for now we don't need it ${GMOCK_LIBRARIES}
)

# add itself to the tests
add_test(test_tree test_tree)
install(TARGETS test_tree DESTINATION bin)