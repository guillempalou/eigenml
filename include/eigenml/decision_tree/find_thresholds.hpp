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
                                                     const IdxVector& samples,
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
                                                     const IdxVector& samples,
                                                     const IdxVector& sorted_index, 
                                                     CriterionType& criterion) {

            size_t n_samples = samples.size();

            // vector that contains the samples sorted by feature value
            IdxVector idx(samples);

            // sort samples according to feature values
            sort(idx.begin(), idx.end(), [&sorted_index](int a, int b){return sorted_index[a] < sorted_index[b];});

            // order the targets and compute a global histogram 
            DistributionType histogram;

            for (size_t i = 0; i < n_samples; ++i) {
                histogram[target(samples[i])]++;
            }

            distribution_ = histogram;

            // we are only interested in the value in the root
            double root_cost = criterion(histogram).first;

            DistributionType left_histogram;

            size_t best_index = 0;
            double best_gain = -1;
            double best_threshold = 0;

            for (size_t i = 0; i < n_samples; ++i) {
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

    // Specialization: Threshold finding for regression
    // We specialize it because entropy, gini and mse are different algorithms, but we want the same interface
    template<class FeatureMatrix, class TargetMatrix>
    class ThresholdFinder<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix> {

        typedef typename TreeTraits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix>::DistributionType DistributionType;
        typedef typename TreeTraits<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;

        static logging::Logger logger;
        
    public:

        ThresholdSplit find_classification_threshold(const FeatureMatrix& features, 
                                                     const TargetMatrix& target, 
                                                     size_t feature_col,
                                                     const IdxVector& samples,
                                                     const IdxVector& sorted_index, 
                                                     CriterionType& criterion) {

            size_t n_samples = samples.size();

            // vector that contains the samples sorted by feature value
            IdxVector idx(samples);

            // sort samples according to feature values
            sort(idx.begin(), idx.end(), [&sorted_index](int a, int b){return sorted_index[a] < sorted_index[b];});

            // order the targets and compute a global histogram 
            DistributionType sum = 0;
            DistributionType sum_x2 = 0;
            for (size_t i = 0; i < n_samples; ++i) {
                sum += target(samples[i]);
                sum_x2 += target(samples[i])*target(samples[i]);
            }
            
            // we are only interested in the value in the root
            double root_cost = criterion(sum_x2/n_samples, sum/n_samples, n_samples).first;

            DistributionType left_sum = 0;
            DistributionType left_sum2 = 0;

            size_t best_index = 0;
            double best_gain = -1;
            double best_threshold = 0;

            for (size_t i = 0; i < n_samples; ++i) {
                double threshold = features(idx[i]);
                double target_i = target(idx[i]);

                left_sum += target_i++;
                left_sum2 += target_i*target_i;
                sum -= target_i;
                sum_x2 -= target_i*target_i;

                auto left_cost = criterion(left_sum2/i, left_sum/i, i);
                auto right_cost = criterion(sum/(n_samples-i-1), sum_x2/(n_samples-i-1), n_samples-i-1);

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

    template<class FeatureMatrix, class TargetMatrix>
    logging::Logger ThresholdFinder<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("ThresholdFinder");

    template<class FeatureMatrix, class TargetMatrix>
    logging::Logger ThresholdFinder<ModelType::kSupervisedRegressor, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("ThresholdFinder");
}}

#endif // FIND_THRESHOLDS_HPP