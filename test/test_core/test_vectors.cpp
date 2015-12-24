#include <gtest/gtest.h>
#include <eigenml/core/vectors/sorting.hpp>

using namespace eigenml;
using namespace eigenml::core;

TEST(ArgSortTest, AssertionTrue) {
    Vector v(3);
    v << 1, 4, 2;

    std::vector<size_t> idx {0, 2, 1};
    std::vector<size_t> sorted = argsort(v);

    ASSERT_EQ(idx, sorted);
}