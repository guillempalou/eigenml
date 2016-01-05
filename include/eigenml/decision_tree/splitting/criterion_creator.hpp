#include <eigenml/core/eigenml.hpp>
#include <eigenml/decision_tree/splitting/criteria.hpp>

namespace eigenml { namespace decision_tree{
    
    // specialization for criteria
    // one for classification, one for regression
    template<ModelType modelType, class DistributionType, class CriterionType, class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator {
        static CriterionType create_criterion_function(SplitCriterion criterion);
    };

    template<typename DistributionType, class CriterionType, class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator<ModelType::kSupervisedClassifier, DistributionType, CriterionType, FeatureMatrix, TargetMatrix> {

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

    template<typename DistributionType, class CriterionType, class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator<kSupervisedRegressor, DistributionType, CriterionType, FeatureMatrix, TargetMatrix> {
        
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