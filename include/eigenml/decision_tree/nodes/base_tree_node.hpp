#ifndef BASE_TREE_NODE_HPP
#define BASE_TREE_NODE_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>

#include <eigenml/core/eigenml.hpp>

#include <eigenml/decision_tree/decision_tree_params.hpp>
#include <eigenml/decision_tree/splitting/find_thresholds.hpp>
#include <eigenml/decision_tree/splitting/criterion_creator.hpp>

namespace eigenml { namespace decision_tree{

    // TODO think about traits only having Node, Criterion and Distribution

    template<ModelType modelType, class DistributionType, class CriterionType, class FeatureMatrix, class TargetMatrix> 
    class BaseTreeNode {

        // type of this class
        typedef BaseTreeNode<modelType, DistributionType, CriterionType, FeatureMatrix, TargetMatrix> NodeType;

        // type of threshold finder
        typedef ThresholdFinder<DistributionType, CriterionType, FeatureMatrix, TargetMatrix> ThresholdFinderType;

        // traits types
        typedef model_traits<modelType, FeatureMatrix, TargetMatrix> model_traits_type;

        // type of an example
        typedef typename model_traits_type::SampleType SampleType;
        typedef typename model_traits_type::ConstSampleType ConstSampleType;

        // logger
        static logging::Logger logger;

    public:

        BaseTreeNode(const DecisionTreeParams& params, 
                     const int& depth, 
                     const ThresholdFinderType& finder,
                     CriterionType& criterion) : params_(params), depth_(depth), finder_(finder), criterion_(criterion) {
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
            bool should_split = recurse &  (params_.max_depth > depth_) & (params_.min_samples < n_samples_);

            for (size_t i = 0; i < n_samples_; ++i)
                distribution_.add_sample(Y(samples[i]), 1); // TODO add weight

            if (!should_split)
                return false;

            for (int c = 0; c < X.cols(); ++c) {

                LOG_TRACE << "Getting optimal threshold for column " << c; 
                // find the optimal threshold on the examples we have
                ThresholdSplit split = finder_.find_threshold(X.col(c), Y, c,
                                                              samples,
                                                              sorted_indexes[c],
                                                              criterion_);

                LOG_TRACE << "Threshold at " << split.threshold << " gain " << split.gain << " column " << split.feature_index;

                if (best_split_ < split) 
                    best_split_ = split;
            }

            LOG_DEBUG << "Optimal threshold at " << best_split_.threshold \
            << " gain " << best_split_.gain \
            << " column " << best_split_.feature_index;

            LOG_TRACE << "Will recurse: " << (best_split_.gain > 0);

            // split the node if we should
            if (best_split_.gain > 0) {

                left_child_ = std::make_shared<NodeType>(params_, depth_+1, finder_, criterion_);
                right_child_ = std::make_shared<NodeType>(params_, depth_+1, finder_, criterion_);

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
                return distribution_.prediction(sample);
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

        const ThresholdFinderType& finder_;
        CriterionType& criterion_;

        size_t n_samples_;

        ThresholdSplit best_split_;
        DistributionType distribution_;

        std::shared_ptr<NodeType> right_child_;
        std::shared_ptr<NodeType> left_child_;
    };


    template<ModelType modelType, class DistributionType, class CriterionType, class FeatureMatrix, class TargetMatrix>
    logging::Logger BaseTreeNode<modelType, DistributionType, CriterionType, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("BaseTreeNode");
    
}}


#endif // BASE_TREE_NODE_HPP