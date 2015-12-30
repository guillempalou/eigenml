#include <eigenml/core/eigenml.hpp>

namespace eigenml { namespace decision_tree {

    template<class Hist>
    ValueAndWeight entropy(const Hist& histogram) {
        double n = 0;
            // count elements on the histogram
        for (auto& e: histogram)
            n+=e.second;
        if (n == 0) 
            return ValueAndWeight(0, 0);
        double ent = 0;
        for (auto& e: histogram) {
            double p = e.second*1.0/n;
            ent += (p != 0) ? -p*std::log2(p) : 0;
        }
        return ValueAndWeight(ent, n);
    }

    template<class Hist>
    ValueAndWeight gini(const Hist& histogram) {
        double n = 0;
        for (auto& e: histogram)
            n+=e.second;
        if (n == 0)
            return ValueAndWeight(0, 0);
        double gini = 0;
        for (auto& e: histogram) {
            double p = e.second*1.0/n;
            gini += p*(1-p);
        }
        return ValueAndWeight(gini, n);
    }

    template<typename T>
    ValueAndWeight mse(const T& mu_x2, const T& mu_x, const T& weight) {
        return ValueAndWeight(mu_x2 - mu_x*mu_x, weight);
    }


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