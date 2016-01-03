#ifndef DECISION_TREE_TRAITS_HPP
#define DECISION_TREE_TRAITS_HPP

#include <eigenml/core/eigenml.hpp>
#include <eigenml/core/model_traits.hpp>

// tree nodes
#include <eigenml/decision_tree/nodes/classification_tree_node.hpp>
#include <eigenml/decision_tree/nodes/regression_tree_node.hpp>

// tree distributions
#include <eigenml/decision_tree/distributions/discrete_distribution.hpp>
#include <eigenml/decision_tree/distributions/continuous_distribution.hpp>

namespace eigenml { namespace decision_tree{

    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    struct tree_traits {
    };

    // if the tree is a classification, we characterize each node by it's histogram
    template<class FeatureMatrix, class TargetMatrix>
    struct tree_traits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix> {

        // get types from the model traits
        typedef model_traits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix> model_traits_type;
        typedef typename model_traits_type::FeatureScalarType ScalarType;

        // node type
        typedef ClassificationTreeNode<FeatureMatrix, TargetMatrix> NodeType;

        // distribution of the nodes
        typedef DiscreteDistribution<ScalarType> DistributionType;

        // criteria function type for splitting nodes
        typedef std::function<ValueAndWeight(const DistributionType&)> CriterionType;
    };

    // if the tree is a regression, we characterize each node by its mean
    template<class FeatureMatrix, class TargetMatrix>
    struct tree_traits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix> {
        
        // get types from the model traits
        typedef core::model_traits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix> model_traits_type;
        typedef typename model_traits_type::FeatureScalarType ScalarType;

        // node type
        typedef RegreessionTreeNode<FeatureMatrix, TargetMatrix> NodeType;

        // distribution of the nodes
        typedef ContinuousDistribution<ScalarType> DistributionType;

        // criteria function type for splitting nodes
        typedef std::function<ValueAndWeight(const DistributionType&)> CriterionType;
    };

}}


#endif // DECISION_TREE_TRAITS_HPP