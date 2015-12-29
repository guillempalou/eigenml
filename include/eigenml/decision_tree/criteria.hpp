

namespace eigenml { namespace decision_tree {

    // specialization for criteria
    // one for classification, one for regression
    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator {
        typedef typename TreeTraits<modelType, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;
        static CriterionType create_criterion_function(SplitCriterion criterion);
    };

    template<class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix> {
        
        typedef typename TreeTraits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;

        static CriterionType create_criterion_function(SplitCriterion criterion) {
            CriterionType function;
            switch(criterion) {
                case SplitCriterion::kEntropyCriterion:
                    return CriterionType(entropy<Histogram>);
                case SplitCriterion::kGiniCriterion:
                    return CriterionType(gini<Histogram>);
                default:
                    throw core::WrongParametersException(core::ExceptionMessage::kWrongSplitCriterionException);
            }
        }
    };

    template<class FeatureMatrix, class TargetMatrix>
    struct CriterionCreator<kSupervisedRegressor, FeatureMatrix, TargetMatrix> {

        typedef typename TreeTraits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;
        
        static CriterionType create_criterion_function(SplitCriterion criterion) {
            CriterionType function;
            switch(criterion) {
                case SplitCriterion::kMSECriterion:
                    return CriterionType(entropy<Histogram>);
                default:
                    throw core::WrongParametersException(core::ExceptionMessage::kWrongSplitCriterionException);
            }
        }
    };
}}