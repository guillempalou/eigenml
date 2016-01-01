#ifndef FIND_THRESHOLDS_HPP
#define FIND_THRESHOLDS_HPP

#include <eigenml/core/eigenml.hpp>
#include <eigenml/decision_tree/decision_tree_params.hpp>

namespace eigenml { namespace decision_tree {

    // function for classification criteria
    typedef std::function<ValueAndWeight(const Histogram& )> Criterion;

    // structure that the split function returns
    struct ThresholdSplit {
        size_t feature_index;
        size_t threshold_index;
        double threshold;
        double gain;    

        template<typename T>
        bool is_right(const T& sample) {
            if (sample(feature_index) > threshold)
                return true;
            return false;
        } 

        bool operator<(const ThresholdSplit& other) {
            return gain < other.gain;
        }
    };

    // generic class to find thresholds
    // specializations in CPP
    template<ModelType modelType, class FeatureMatrix, class TargetMatrix>
    class ThresholdFinder {
    
        typedef typename TreeTraits<modelType, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;

    public:

        ThresholdSplit find_classification_threshold(const FeatureMatrix& features, 
                                                     const TargetMatrix& target, 
                                                     size_t feature_col,
                                                     const IdxVector& examples,
                                                     const IdxVector& sorted_index, 
                                                     CriterionType& criterion);

        typename TreeTraits<modelType, FeatureMatrix, TargetMatrix>::DistributionType node_distribution();
    };

    // Specialization: Threshold finding for classification
    // We specialize it because entropy, gini and mse are different algorithms, but we want the same interface
    template<class FeatureMatrix, class TargetMatrix>
    class ThresholdFinder<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix> {

        typedef typename TreeTraits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::DistributionType DistributionType;
        typedef typename TreeTraits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;

        static logging::Logger logger;

    public:

        ThresholdSplit find_classification_threshold(const FeatureMatrix& features, 
                                                     const TargetMatrix& target, 
                                                     size_t feature_col,
                                                     const IdxVector& examples,
                                                     const IdxVector& sorted_index, 
                                                     CriterionType& criterion) {

            size_t n_examples = examples.size();

            // vector that contains the examples sorted by feature value
            IdxVector idx(examples);

            // sort examples according to feature values
            sort(idx.begin(), idx.end(), [&sorted_index](int a, int b){return sorted_index[a] < sorted_index[b];});

            // order the targets and compute a global histogram 
            DistributionType histogram;

            for (size_t i = 0; i < n_examples; ++i) {
                histogram[target(examples[i])]++;
            }

            distribution_ = histogram;

            // we are only interested in the value in the root
            double root_cost = criterion(histogram).first;

            DistributionType left_histogram;

            size_t best_index = 0;
            double best_gain = -1;
            double best_threshold = 0;

            for (size_t i = 0; i < n_examples; ++i) {
                double threshold = features(idx[i]);
                double target_i = target(idx[i]);
                left_histogram[target_i]++;
                histogram[target_i]--;

                auto left_cost = criterion(left_histogram);
                auto right_cost = criterion(histogram);

                // get the weight for each child and for the root
                double w = left_cost.second + right_cost.second;
                double w_left = left_cost.second;
                double w_right = right_cost.second;

                    //compute the gain
                double gain = w * root_cost - w_left * left_cost.first - w_right * right_cost.first;

                if (best_gain < gain) {
                    best_index = i;
                    best_threshold = threshold;
                    best_gain = gain;
                }
            }

            ThresholdSplit split = {feature_col, best_index, best_threshold, best_gain};
            return split;
        }

        const DistributionType node_distribution() {
            return distribution_;
        }

    protected:

        DistributionType distribution_;

    };

    // // Specialization: Threshold finding for regression
    // // We specialize it because entropy, gini and mse are different algorithms, but we want the same interface
    // template<class FeatureMatrix, class TargetMatrix>
    // class ThresholdFinder<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix> {

    //     typedef TreeTraits<ModelType::kSupervisedRegressor, FeatureMatrix, targetMatrix>::DistributionType DistributionType;

    // public:

    //     ThresholdSplit find_classification_threshold(const FeatureMatrix& features, 
    //                                                  const TargetMatrix& target, 
    //                                                  size_t feature_col,
    //                                                  const IdxVector& examples,
    //                                                  const IdxVector& sorted_index, 
    //                                                  Criterion& criterion) {

    //         size_t n_examples = examples.size();

    //         // vector that contains the examples sorted by feature value
    //         IdxVector idx(examples);

    //         // sort examples according to feature values
    //         sort(idx.begin(), idx.end(), [&sorted_index](int a, int b){return sorted_index[a] < sorted_index[b];});

    //         // order the targets and compute a global histogram 
    //         DistributionType mean;
    //         DistributionType mean_x2;
    //         for (size_t i = 0; i < n_examples; ++i)
    //             mean += target(examples[i])]++;
                
    //         DistributionType var = mean_x2 - mean*mean;

    //         // we are only interested in the value in the root
    //         double root_cost = criterion(histogram).first;

    //         Histogram left_histogram;

    //         size_t best_index = 0;
    //         double best_gain = -1;
    //         double best_threshold = 0;

    //         for (size_t i = 0; i < n_examples; ++i) {
    //             double threshold = features(idx[i]);
    //             double target_i = target(idx[i]);
    //             left_histogram[target_i]++;
    //             histogram[target_i]--;

    //             auto left_cost = criterion(left_histogram);
    //             auto right_cost = criterion(histogram);

    //             // get the weight for each child and for the root
    //             double w = left_cost.second + right_cost.second;
    //             double w_left = left_cost.second;
    //             double w_right = right_cost.second;

    //                 //compute the gain
    //             double gain = w * root_cost - w_left * left_cost.first - w_right * right_cost.first;

    //             if (best_gain < gain) {
    //                 best_index = i;
    //                 best_threshold = threshold;
    //                 best_gain = gain;
    //             }
    //         }

    //         ThresholdSplit split = {feature_col, best_index, best_threshold, best_gain, max_class, 0};
    //         return split;
    //     }

    //     const DistributionType node_distribution() {
    //         return distribution_;
    //     }

    // protected:

    //     DistributionType distribution_;

    // };

    template<class FeatureMatrix, class TargetMatrix>
    logging::Logger ThresholdFinder<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("ThresholdFinder");

}}

#endif // FIND_THRESHOLDS_HPP