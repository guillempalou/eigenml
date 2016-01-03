#include <eigenml/decision_tree/distributions/base_distribution.hpp>

namespace eigenml { namespace decision_tree{
    template<typename T>
    class DiscreteDistribution : public BaseDistribution<T> {

        typedef BaseDistribution<T> BaseType;

        typedef std::map<T, double> HistogramType;

    public:
        
        void add_sample(const T& target_value, const double weight) {
            histogram_[target_value]+=weight;
            update_moments(target_value, weight);
            update_prediction(target_value, weight);
        }

        void remove_sample(const T& target_value, const double weight) {
            histogram_[target_value]-=weight;
            // TODO check histogram value asserting positivity
            // update moments with the negative weight to remove
            update_moments(target_value, -weight);
            update_prediction(target_value, -weight);
        }   

        void update_prediction(const T& target_value, const double weight) {
            // check if we add a sample
            if ((weight > 0) && (histogram_[target_value] > max_class_weight_)) {
                max_class_weight_ = histogram_[target_value];
                max_class_ = target_value;
            } 

            // check only changes if we remove a sample from the maximum class
            if ((weight < 0) && (target_value == max_class_)) {
                max_class_weight_ = -1;
                for (auto& x: histogram_) {
                    if (x.second > max_class_weight_) {
                        max_class_weight_ = x.second;
                        max_class_ = x.first;
                    }
                }
            }
        }

        const HistogramType& histogram() const {
            return histogram_;
        }

        // function for predicting 
        // overload parent that returns the mean
        template <typename U>
        const T prediction(const U& sample) {
            return max_class_;
        }

    protected:

        T max_class_weight_;
        T max_class_;

        HistogramType histogram_;
    };
}}