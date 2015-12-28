#ifndef DECISION_TREE_NODE_HPP
#define DECISION_TREE_NODE_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>

#include <eigenml/core/eigenml.hpp>

#include <eigenml/decision_tree/decision_tree_params.hpp>
#include <eigenml/decision_tree/find_thresholds.hpp>

namespace eigenml { namespace decision_tree {

    template<class FeatureMatrix, class TargetMatrix> 
    class DecisionTreeNode {

        // type of this class
        typedef DecisionTreeNode<FeatureMatrix, TargetMatrix> NodeType;

        // logger
        static logging::Logger logger;

    public:

        DecisionTreeNode(const DecisionTreeParams& params, const int& depth) : params(params), depth(depth) {
            LOG_DEBUG << params.criterion;
            switch(params.criterion) {
                case SplitCriterion::ENTROPY_CRITERION:
                    criterion = Criterion(entropy);
                    break;
                case SplitCriterion::GINI_CRITERION:
                    criterion = Criterion(gini);
                    break;
                default:
                    throw core::EigenMLException("Wrong node split criterion");
            }
        }

        bool split(const FeatureMatrix& X, const TargetMatrix& Y, 
                   IdxVector examples,
                   std::vector<IdxVector> sorted_indexes, // TODO add subset of features
                   bool recurse) {

            size_t n_examples = examples.size();

            LOG_DEBUG << "Node with " << n_examples << " examples at depth " << depth;
            LOG_DEBUG << "Examples: " << logging::vector_str(examples);

            // if the depth is sufficient
            if (params.max_depth == depth) 
                return false;

            if (params.min_examples >= n_examples) 
                return false;

            // find the column that best splits the node
            ThresholdSplit best_split = {0, 0, 0, -1e10};

            for (int c = 0; c < X.cols(); ++c) {

                LOG_TRACE << "Getting optimal threshold for column " << c; 
                // find the optimal threshold on the examples we have
                ThresholdSplit split = find_classification_threshold(X.col(c), Y, c,
                                                                     examples,
                                                                     sorted_indexes[c],
                                                                     criterion);

                LOG_TRACE << "Threshold at " << split.threshold << " gain " \
                              << split.gain << " column " << split.feature_index;

                if (best_split < split) {
                    best_split = split;
                }
            }

            LOG_DEBUG << "Optimal threshold at " << best_split.threshold << " gain " \
                              << best_split.gain << " column " << best_split.feature_index;

            // add a bunch of conditions for splitting
            bool should_split = recurse & 
                                (best_split.gain > 0) & 
                                (params.max_depth < depth) &
                                (params.min_examples < n_examples);

            // split the node if we should
            if (should_split) {
                left_child = std::make_shared<NodeType>(params, depth+1);
                right_child = std::make_shared<NodeType>(params, depth+1);

                // get the examples based on feature threshold
                IdxVector left_examples;
                IdxVector right_examples;
                for (size_t k = 0; k < n_examples; k++) {
                    if (X.col(best_split.feature_index)(examples[k]) <= best_split.threshold)
                        left_examples.push_back(examples[k]);
                    else
                        right_examples.push_back(examples[k]);
                }
                LOG_DEBUG << "Left node has " << left_examples.size();
                LOG_DEBUG << "Right node has " << right_examples.size() << " examples";

                bool lc = left_child->split(X, Y, left_examples, sorted_indexes, true);
                bool rc = right_child->split(X, Y, right_examples, sorted_indexes, true);
                return lc & rc;
            }
            
            return false;
        }

    protected:

        const DecisionTreeParams& params;
        
        size_t depth;
        size_t n_examples;

        ThresholdSplit best_split;
        Criterion criterion;

        std::shared_ptr<NodeType> right_child;
        std::shared_ptr<NodeType> left_child;
    };

    template<class FeatureMatrix, class TargetMatrix>
    logging::Logger DecisionTreeNode<FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("DecisionTreeNode");

}}

#endif
