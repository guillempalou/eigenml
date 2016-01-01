#include <gtest/gtest.h>
#include <eigenml/decision_tree/find_thresholds.hpp>
#include <eigenml/decision_tree/criteria.hpp>

using namespace eigenml;
using namespace eigenml::decision_tree;

TEST(ThresholdFinding, Entropy) {
    Histogram hist;
    hist[0] = 10;
    hist[1] = 10;
    ValueAndWeight v = entropy(hist);
    ASSERT_DOUBLE_EQ(v.first, 1);
    ASSERT_DOUBLE_EQ(v.second, 20);
}

TEST(ThresholdFinding, Gini) {
    Histogram hist;
    hist[0] = 10;
    hist[1] = 10;
    ValueAndWeight v = gini(hist);
    ASSERT_DOUBLE_EQ(v.first, 0.5);
    ASSERT_DOUBLE_EQ(v.second, 20);
}

TEST(ThresholdFinding, SimplethresholdEntropy) {
    size_t N = 2;
    Matrix X(N, 1);
    Vector Y(N);

    X << 1, 2;
    Y << 0, 1;

    IdxVector idx{0, 1};
    IdxVector sorted{0, 1};

    auto criterion = Criterion(entropy<Histogram>);

    ThresholdFinder<ModelType::kSupervisedClassifier, Matrix, Vector> splitter;
    ThresholdSplit split = splitter.find_classification_threshold(X, Y, 0, idx, sorted, criterion);

    ASSERT_DOUBLE_EQ(1, split.threshold);
    ASSERT_DOUBLE_EQ(2, split.gain);
    ASSERT_EQ(0, split.feature_index);
    ASSERT_EQ(0, split.threshold_index);
}

TEST(ThresholdFinding, SimplethresholdGini) {
    size_t N = 2;
    Matrix X(N, 1);
    Vector Y(N);

    X << 1, 2;
    Y << 0, 1;

    IdxVector idx{0, 1};
    IdxVector sorted{0, 1};

    auto criterion = Criterion(gini<Histogram>);

    ThresholdFinder<ModelType::kSupervisedClassifier, Matrix, Vector> splitter;
    ThresholdSplit split = splitter.find_classification_threshold(X, Y, 0, idx, sorted, criterion);

    ASSERT_DOUBLE_EQ(1, split.threshold);
    ASSERT_DOUBLE_EQ(1, split.gain);
    ASSERT_EQ(0, split.feature_index);
    ASSERT_EQ(0, split.threshold_index);
}