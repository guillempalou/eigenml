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

    template<ModelType modelType, class FeatureMatrix, class TargetMatrix> 
    class DecisionTreeNode {

        // type of this class
        typedef DecisionTreeNode<modelType, FeatureMatrix, TargetMatrix> NodeType;

        // logger
        static logging::Logger logger;

    public:

        DecisionTreeNode(const DecisionTreeParams& params, const int& depth) : params_(params), depth_(depth) {
            LOG_DEBUG << params.criterion;
            switch(params.criterion) {
                case SplitCriterion::kEntropyCriterion:
                    criterion_ = Criterion(entropy<Histogram>);
                    break;
                case SplitCriterion::kGiniCriterion:
                    criterion_ = Criterion(gini<Histogram>);
                    break;
                default:
                    throw core::WrongParametersException(core::ExceptionMessage::kWrongSplitCriterionException);
            }
        }

        bool split(const FeatureMatrix& X, const TargetMatrix& Y, 
                   IdxVector samples,
                   std::vector<IdxVector> sorted_indexes, // TODO add subset of features
                   bool recurse) {

            n_samples_ = samples.size();

            LOG_DEBUG << "Node with " << n_samples_ << " samples at depth " << depth_;
            LOG_TRACE << "Examples: " << logging::vector_str(samples);

            // find the column that best splits the node
            best_split_ = {0, 0, 0, -1e10};

            // add a bunch of conditions for splitting
            bool should_split = recurse & 
                                (params_.max_depth > depth_) &
                                (params_.min_samples < n_samples_);

            if (should_split == false || params_.save_distributions) {

                if (!should_split)
                    return false;
            }

            for (int c = 0; c < X.cols(); ++c) {

                LOG_TRACE << "Getting optimal threshold for column " << c; 
                // find the optimal threshold on the examples we have
                ThresholdSplit split = find_classification_threshold(X.col(c), Y, c,
                                                                     samples,
                                                                     sorted_indexes[c],
                                                                     criterion_);

                LOG_TRACE << "Threshold at " << split.threshold << " gain " \
                              << split.gain << " column " << split.feature_index;

                if (best_split_ < split) {
                    best_split_ = split;
                }
            }

            LOG_DEBUG << "Optimal threshold at " << best_split_.threshold \
                        << " gain " << best_split_.gain \
                        << " column " << best_split_.feature_index;

            

            LOG_TRACE << "Will recurse: " << should_split;

            should_split &= best_split_.gain > 0;

            // split the node if we should
            if (should_split) {
                left_child_ = std::make_shared<NodeType>(params_, depth_+1);
                right_child_ = std::make_shared<NodeType>(params_, depth_+1);

                // get the examples based on feature threshold
                IdxVector left_samples;
                IdxVector right_samples;
                for (size_t k = 0; k < n_samples_; k++) {
                    if (X.col(best_split_.feature_index)(samples[k]) <= best_split_.threshold)
                        left_samples.push_back(samples[k]);
                    else
                        right_samples.push_back(samples[k]);
                }
                LOG_DEBUG << "Left node has " << left_samples.size() << " samples ";
                LOG_DEBUG << "Right node has " << right_samples.size() << " samples";

                bool lc = left_child_->split(X, Y, left_samples, sorted_indexes, true);
                bool rc = right_child_->split(X, Y, right_samples, sorted_indexes, true);
                return lc & rc;
            }
            
            return false;
        }

        size_t depth() {
            return depth_;
        }

        size_t n_samples() {
            return n_samples_;
        }

        const NodeType& right_child() {
            return *right_child_.get();
        }

        const NodeType& left_child() {
            return *left_child_.get();
        }

    protected:

        const DecisionTreeParams& params_;
        
        size_t depth_;
        size_t n_samples_;

        ThresholdSplit best_split_;
        Criterion criterion_;
        // TODO add variables to capture state

        std::shared_ptr<NodeType> right_child_;
        std::shared_ptr<NodeType> left_child_;
    };

    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    logging::Logger DecisionTreeNode<modelType, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("DecisionTreeNode");

}}

#endif
