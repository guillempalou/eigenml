#ifndef CONTINUOUS_DISTRIBUTION_HPP
#define CONTINUOUS_DISTRIBUTION_HPP

#include <eigenml/decision_tree/distributions/base_distribution.hpp>

namespace eigenml { namespace decision_tree{

    template<typename T>
    class ContinuousDistribution : public BaseDistribution<T> {

        typedef BaseDistribution<T> BaseType;

    public:
        
        void add_sample(const T& target_value, const double weight) {
            BaseType::update_moments(target_value, weight);
        }

        void remove_sample(const T& target_value, const double weight) {
            BaseType::update_moments(target_value, -weight);
        }

        // function for predicting 
        template <typename U>
        const T prediction(const U& sample) {
            return BaseType::mean_;
        }

    protected:

    };

}}


#endif // CONTINUOUS_DISTRIBUTION_HPP