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

        bool operator<(const ThresholdSplit& other) {
            return gain < other.gain;
        }
    };

    // TODO move entropy to metrics module
    //TODO find_regression_threshold
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

    template<class FeatureMatrix, class TargetMatrix>
    class ThresholdFinder<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix> {

        typedef typename TreeTraits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::DistributionType DistributionType;
        typedef typename TreeTraits<ModelType::kSupervisedClassifier, FeatureMatrix, TargetMatrix>::CriterionType CriterionType;

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
            for (size_t i = 0; i < n_examples; ++i)
                histogram[target(examples[i])]++;

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

}}

#endif // FIND_THRESHOLDS_HPP