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

}}

#endif