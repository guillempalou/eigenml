#ifndef BASE_DISTRIBUTION_HPP
#define BASE_DISTRIBUTION_HPP

#include <cmath>
#include <map>

#include <eigenml/core/eigenml.hpp>

namespace eigenml { namespace decision_tree{

    template<typename T> 
    class BaseDistribution {

    public:

        virtual void add_sample(const T& target_value, const double weight) = 0;
        virtual void remove_sample(const T& target_value, const double weight) = 0;

        // function for predicting 
        template <typename U>
        const T prediction(const U& sample = nullptr) {
            throw core::MethodNotImplementedException("prediction");
        };

        // mean of the distribution
        virtual const T mean() const {
            return mean_;
        }

        // momentum of order 2 (convenience function because it can be efficiently implemented) 
        virtual const T m2() const {
            return m2_;
        }
        
        // variance of the distribution
        const T var() const {
            return m2() - mean()*mean();
        }

        // standard deviation of the distribution
        const T std() const {
            return std::sqrt(var());
        }

        const T weight() const {
            return total_weight_;
        }

    protected:

        void update_moments(const T& target_value, const double weight){
            sum_+=target_value*weight;
            sum2_+=target_value*target_value*weight;
            total_weight_+=weight;
            mean_ = sum_ / total_weight_;
            m2_ = sum2_ / total_weight_;
        }

        T mean_;
        T m2_;
        T sum_;
        T sum2_;
        T total_weight_;
    };

}}

#endif // BASE_DISTRIBUTION_HPP