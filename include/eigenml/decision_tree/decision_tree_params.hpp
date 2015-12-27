#ifndef __DECISION_TREE_PARAMS_HPP__
#define __DECISION_TREE_PARAMS_HPP__

#include <string>

namespace eigenml { namespace decision_tree {

    enum SplitCriterion {
        ENTROPY_CRITERION,
        GINI_CRITERION,
        MSE_CRITERION
    };

    struct DecisionTreeParams {

        static const int DEFAULT_MAX_DEPTH = 5;
        static const int DEFAULT_MIN_EXAMPLES = 1;
        static const SplitCriterion DEFAULT_CRITERION = SplitCriterion::ENTROPY_CRITERION;

        SplitCriterion criterion = DEFAULT_CRITERION;
        size_t max_depth = DEFAULT_MAX_DEPTH;
        size_t min_examples = DEFAULT_MIN_EXAMPLES;
    };

}}

#endif