#ifndef __DECISION_TREE_PARAMS_HPP__
#define __DECISION_TREE_PARAMS_HPP__

#include <string>

namespace eigenml { namespace decision_tree {

    enum SplitCriterion {
        kEntropyCriterion,
        kGiniCriterion,
        kMSECriterion
    };

    struct DecisionTreeParams {

        static const int kDefaultMaxDepth = 5;
        static const int kDefaultMinSamples = 1;
        static const SplitCriterion kDefaultCriterion = SplitCriterion::kGiniCriterion;

        size_t max_depth = kDefaultMaxDepth;
        SplitCriterion criterion = kDefaultCriterion;
        size_t min_samples = kDefaultMinSamples;
        bool save_distributions;    
    };

    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    struct TreeTraits {
    };

    // if the tree is a classification, we characterize each node by it's histogram
    template<class FeatureMatrix, class TargetMatrix>
    struct TreeTraits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix> {
        typedef Histogram DistributionType;
        typedef std::function<ValueAndWeight(const DistributionType&)> CriterionType;
    };

    // if the tree is a regression, we characterize each node by its mean
    template<class FeatureMatrix, class TargetMatrix>
    struct TreeTraits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix> {
        typedef double DistributionType;
        typedef std::function<ValueAndWeight(const double&, const double&, const double&)> CriterionType;
    };

}}

#endif