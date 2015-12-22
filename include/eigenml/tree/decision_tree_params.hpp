#ifndef __DECISION_TREE_PARAMS_HPP__
#define __DECISION_TREE_PARAMS_HPP__

#include <string>

struct DecisionTreeParams {

    std::string criterion;
    int max_depth;
    int min_examples;

    static const int DEFAULT_MAX_DEPTH = 5;
    static const int DEFAULT_MIN_EXAMPLES = 1;

    // default constructor with default values
    DecisionTreeParams() : max_depth(DEFAULT_MAX_DEPTH), 
                           min_examples(DEFAULT_MIN_EXAMPLES) {
    }

};

#endif