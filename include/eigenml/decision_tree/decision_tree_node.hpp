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
#include <eigenml/decision_tree/criteria.hpp>

namespace eigenml { namespace decision_tree {

    template<ModelType modelType, class FeatureMatrix, class TargetMatrix> 
    class DecisionTreeNode {

        // type of an example
        typedef typename FeatureMatrix::RowXpr SampleType;
        typedef typename FeatureMatrix::ConstRowXpr ConstSampleType;

        // type of this class
        typedef DecisionTreeNode<modelType, FeatureMatrix, TargetMatrix> NodeType;

        // distribution on each node
        typedef typename TreeTraits<modelType, FeatureMatrix, TargetMatrix>::DistributionType DistributionType;

        // Threshold finder type
        typedef ThresholdFinder<modelType, FeatureMatrix, TargetMatrix> ThresholdFinderType;

        // criterion type
        typedef typename TreeTraits<modelType, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;

        // creation of criteria
        typedef CriterionCreator<modelType, FeatureMatrix, TargetMatrix> CriterionCreatorType;

        // logger
        static logging::Logger logger;

    public:

        DecisionTreeNode(const DecisionTreeParams& params, const int& depth) : params_(params), depth_(depth) {
            LOG_DEBUG << "Criterion for creating the tree " << params.criterion;
            criterion_ = CriterionCreatorType::create_criterion_function(params.criterion);
        }

        bool split(const FeatureMatrix& X, const TargetMatrix& Y, 
                   const IdxVector& samples,
                   const std::vector<IdxVector>& sorted_indexes, // TODO add subset of features
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

            Histogram histogram;
            double max_class = 0;
            double n_max_class = 0;
            for (size_t i = 0; i < n_samples_; ++i) {
                histogram[Y(samples[i])]++;
                if (n_max_class < histogram[Y(samples[i])]) {
                    n_max_class = histogram[Y(samples[i])];
                    max_class = Y(samples[i]);
                }
            }
            LOG_DEBUG << "Max Class in node: " << max_class << " with " << n_max_class << " samples";
            decision_ = max_class;

            if (should_split == false || params_.save_distributions) {

                if (!should_split)
                    return false;
            }

            ThresholdFinderType splitter;

            for (int c = 0; c < X.cols(); ++c) {

                LOG_TRACE << "Getting optimal threshold for column " << c; 
                // find the optimal threshold on the examples we have
                ThresholdSplit split = splitter.find_classification_threshold(X.col(c), Y, c,
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

            // capture node distribution
            if (params_.save_distributions || should_split == false)
                distribution_  = splitter.node_distribution();

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

        double predict(const ConstSampleType& sample) {
            if (right_child_.get() == nullptr) {
                return decision_;
            } else {
                if (best_split_.is_right(sample) == true)
                    return right_child_.get()->predict(sample);
                else 
                    return left_child_.get()->predict(sample);
            }
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
        CriterionType criterion_;

        double decision_;

        DistributionType distribution_;

        std::shared_ptr<NodeType> right_child_;
        std::shared_ptr<NodeType> left_child_;
    };


    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    logging::Logger DecisionTreeNode<modelType, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("DecisionTreeNode");
}}

#endif
