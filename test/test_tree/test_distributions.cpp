#include <gtest/gtest.h>
#include <eigenml/decision_tree/distributions/discrete_distribution.hpp>
#include <eigenml/decision_tree/distributions/continuous_distribution.hpp>

using namespace eigenml;
using namespace eigenml::decision_tree;

TEST(TestDistributions, EasyDiscreteDistribution) {
    DiscreteDistribution<double> hist;
    hist.add_sample(2, 1);
    ASSERT_FLOAT_EQ(2, hist.mean());
    ASSERT_FLOAT_EQ(4, hist.m2());
    ASSERT_FLOAT_EQ(0, hist.var());
}

TEST(TestDistributions, MediumDiscreteDistribution) {
    DiscreteDistribution<double> hist;
    hist.add_sample(2, 1);
    hist.add_sample(3, 1);
    ASSERT_FLOAT_EQ(2.5, hist.mean());
    ASSERT_FLOAT_EQ(6.5, hist.m2());
    ASSERT_FLOAT_EQ(0.25, hist.var());
}

TEST(TestDistributions, HardDiscreteDistribution) {
    DiscreteDistribution<double> hist;
    hist.add_sample(2, 1);
    hist.add_sample(3, 1);
    hist.add_sample(4, 1);
    hist.remove_sample(2, 1);
    ASSERT_FLOAT_EQ(3.5, hist.mean());
    ASSERT_FLOAT_EQ(12.5, hist.m2());
    ASSERT_FLOAT_EQ(0.25, hist.var());
}

TEST(TestDistributions, EasyContinousDistribution) {
    ContinuousDistribution<double> hist;
    hist.add_sample(2, 1);
    ASSERT_FLOAT_EQ(2, hist.mean());
    ASSERT_FLOAT_EQ(4, hist.m2());
    ASSERT_FLOAT_EQ(0, hist.var());
}

TEST(TestDistributions, MediumContinuousDistribution) {
    ContinuousDistribution<double> hist;
    hist.add_sample(2, 1);
    hist.add_sample(3, 1);
    ASSERT_FLOAT_EQ(2.5, hist.mean());
    ASSERT_FLOAT_EQ(6.5, hist.m2());
    ASSERT_FLOAT_EQ(0.25, hist.var());
}

TEST(TestDistributions, HardContinuousDistribution) {
    ContinuousDistribution<double> hist;
    hist.add_sample(2, 1);
    hist.add_sample(3, 1);
    hist.add_sample(4, 1);
    hist.remove_sample(2, 1);
    ASSERT_FLOAT_EQ(3.5, hist.mean());
    ASSERT_FLOAT_EQ(12.5, hist.m2());
    ASSERT_FLOAT_EQ(0.25, hist.var());
}