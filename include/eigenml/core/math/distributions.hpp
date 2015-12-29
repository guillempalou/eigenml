#ifndef DISTRIBUTIONS_HPP
#define DISTRIBUTIONS_HPP

namespace eigenml { namespace core {

    enum DistributionType {
        kDiscreteDistribution,
        kContinousDistribution
    }

    // Distributions for different models
    template <DistributionType type, typename BaseType>
    struct DistributionPDF {

        BaseType mean();
        BaseType std();
    };
    
}}


#endif // DISTRIBUTIONS_HPP
