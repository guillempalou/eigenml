#include <eigenml/core/eigenml.hpp>

#include <eigenml/decision_tree/decision_tree_traits.hpp>
#include <eigenml/decision_tree/splitting/criteria.hpp>

namespace eigenml { namespace decision_tree{
    
    // specialization for criteria
    // one for classification, one for regression
    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator {
        typedef tree_traits<modelType, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;
        static CriterionType create_criterion_function(SplitCriterion criterion);
    };

    template<class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix> {
        
        typedef typename tree_traits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;
        typedef typename tree_traits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::DistributionType DistributionType;

        static CriterionType create_criterion_function(SplitCriterion criterion) {
            CriterionType function;
            switch(criterion) {
                case SplitCriterion::kEntropyCriterion:
                    return CriterionType(entropy<DistributionType>);
                case SplitCriterion::kGiniCriterion:
                    return CriterionType(gini<DistributionType>);
                default:
                    throw core::WrongParametersException(core::ExceptionMessage::kWrongSplitCriterionException);
            }
        }
    };

    template<class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator<kSupervisedRegressor, FeatureMatrix, TargetMatrix> {

        typedef typename tree_traits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;
        typedef typename tree_traits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix>::DistributionType DistributionType;
        
        static CriterionType create_criterion_function(SplitCriterion criterion) {
            CriterionType function;
            switch(criterion) {
                case SplitCriterion::kMSECriterion:
                    return CriterionType(mse<DistributionType>);
                default:
                    throw core::WrongParametersException(core::ExceptionMessage::kWrongSplitCriterionException);
            }
        }
    };

}}