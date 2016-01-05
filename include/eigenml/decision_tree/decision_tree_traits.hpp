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

        // distribution of the nodes
        typedef DiscreteDistribution<ScalarType> DistributionType;

        // criteria function type for splitting nodes
        typedef std::function<ValueAndWeight(const DistributionType&)> CriterionType;

        // Class type to find the optimal split
        //typedef ThresholdFinder<DistributionType, CriterionType, FeatureMatrix, TargetMatrix> ThresholdFinderType;

        // creation of criteria
        //typedef CriterionCreator<ModelType::kSupervisedClassifier, DistributionType, CriterionType, FeatureMatrix, TargetMatrix> CriterionCreatorType;

        // node type
        typedef ClassificationTreeNode<DistributionType, CriterionType, FeatureMatrix, TargetMatrix> NodeType;
    };

    // if the tree is a regression, we characterize each node by its mean
    template<class FeatureMatrix, class TargetMatrix>
    struct tree_traits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix> {
        
        // get types from the model traits
        typedef model_traits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix> model_traits_type;
        typedef typename model_traits_type::FeatureScalarType ScalarType;

        // distribution of the nodes
        typedef ContinuousDistribution<ScalarType> DistributionType;

        // criteria function type for splitting nodes
        typedef std::function<ValueAndWeight(const DistributionType&)> CriterionType;

        // node type
        typedef RegressionTreeNode<DistributionType, CriterionType, FeatureMatrix, TargetMatrix> NodeType;

        // Class type to find the optimal split
        //typedef ThresholdFinder<DistributionType, CriterionType, FeatureMatrix, TargetMatrix> ThresholdFinderType;

        // creation of criteria
        //typedef CriterionCreator<ModelType::kSupervisedRegressor, DistributionType, CriterionType, FeatureMatrix, TargetMatrix> CriterionCreatorType;
    };

}}


#endif // DECISION_TREE_TRAITS_HPP