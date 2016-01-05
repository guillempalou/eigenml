#ifndef FIND_THRESHOLDS_HPP
#define FIND_THRESHOLDS_HPP

#include <eigenml/core/eigenml.hpp>
#include <eigenml/decision_tree/decision_tree_params.hpp>

namespace eigenml { namespace decision_tree {

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
    template<class DistributionType, class CriterionType, class FeatureMatrix, class TargetMatrix>
    class ThresholdFinder {
        
        static logging::Logger logger;

    public:

        const ThresholdSplit find_threshold(const FeatureMatrix& features, 
                                            const TargetMatrix& target, 
                                            size_t feature_col,
                                            const IdxVector& samples,
                                            const IdxVector& sorted_index, 
                                            CriterionType& criterion) const {

            size_t n_samples = samples.size();

            // vector that contains the samples sorted by feature value
            IdxVector idx(samples);

            // sort samples according to feature values
            sort(idx.begin(), idx.end(), [&sorted_index](int a, int b){return sorted_index[a] < sorted_index[b];});

            // order the targets and compute a global histogram 
            DistributionType distribution;
            DistributionType left_distribution;

            for (size_t i = 0; i < n_samples; ++i)
                distribution.add_sample(target(samples[i]), 1);

            // we are only interested in the value in the root
            double root_cost = criterion(distribution).first;

            size_t best_index = 0;
            double best_gain = -1;
            double best_threshold = 0;

            for (size_t i = 0; i < n_samples; ++i) {
                double threshold = features(idx[i]);
                double target_i = target(idx[i]);

                left_distribution.add_sample(target_i, 1);
                distribution.remove_sample(target_i, 1);

                auto left_cost = criterion(left_distribution);
                auto right_cost = criterion(distribution);

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

    protected:
    };

    template<class DistributionType, class CriterionType, class FeatureMatrix, class TargetMatrix>
    logging::Logger ThresholdFinder<DistributionType, CriterionType, FeatureMatrix, TargetMatrix>::logger = logging::setNameAttribute("ThresholdFinder");

}}

#endif // FIND_THRESHOLDS_HPP