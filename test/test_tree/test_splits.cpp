#include <gtest/gtest.h>
#include <eigenml/decision_tree/find_thresholds.hpp>

using namespace eigenml;
using namespace eigenml::decision_tree;

TEST(ThresholdFinding, Simplethreshold) {
    size_t N = 2;
    Matrix X(N, 1);
    Vector Y(N);

    X << 1, 2;
    Y << 0, 1;

    IdxVector idx{0, 1};
    IdxVector sorted{0, 1};

    auto criterion = Criterion(entropy);

    ThresholdSplit split = find_classification_threshold(X, Y, 0, idx, sorted, criterion);

    ASSERT_EQ(1, split.threshold);
    ASSERT_EQ(2, split.gain);
    ASSERT_EQ(0, split.feature_index);
    ASSERT_EQ(0, split.threshold_index);
}